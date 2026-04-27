import pickle
import os
import os.path
import uuid

from typing_extensions import Self
from typing import List, BinaryIO, Tuple, Dict

from sqlalchemy.orm import Session
from sqlalchemy.sql import expression
from sqlalchemy.exc import IntegrityError

from app.db import models, schemas
from app.core.image_processing import ImageProcessing
from app.cache import client as cache_client

from datetime import datetime
from datetime import timedelta


class DuplicateError(BaseException):
    pass


class UserService:
    def __init__(
        self: Self, image_processor: ImageProcessing, access_directory: str
    ) -> Self:
        self.image_processor = image_processor
        self.access_directory = access_directory

        try:
            os.mkdir(access_directory)
        except FileExistsError:
            pass

    @staticmethod
    def get_users(
        db: Session, skip: int = 0, limit: int = 100
    ) -> List[schemas.UserRead]:
        users = db.query(models.User).offset(skip).limit(limit).all()
        return users

    def create_user(
        self: Self, db: Session, user: schemas.UserCreate, files: List[BinaryIO]
    ):

        images, encodings = self.image_processor.register(user.username, files)

        try:
            db_user = models.User(
                username=user.username,
                first_name=user.first_name,
                last_name=user.last_name,
                email=user.email,
                is_active=expression.true(),
            )
            encodings = [
                models.Encoding(data=pickle.dumps(encoding), owner_id=db_user.id)
                for encoding in encodings
            ]
            db_user.encodings = encodings
            db.add(db_user)
            cache_client.delete("encodings")
            db.commit()
        except IntegrityError:
            raise DuplicateError()

        return images

    def login(self: Self, db: Session, buffer: BinaryIO) -> Tuple[any, str or None]:
        encodings = UserService.__get_face_encodings__(db)
        return self.image_processor.authenticate(buffer, encodings)

    def register_access(
        self: Self, db: Session, access: schemas.AccessCreate, file: BinaryIO
    ):
        img_name: str = f"{uuid.uuid4().hex}.jpg"
        p = os.path.join(self.access_directory, f"{img_name}")
        ret = self.image_processor.to_file(p, file)

        if not ret:
            raise Exception(f"cannot save access on file {p}")

        try:
            if len(access.users) != 0 or any(user is not None for user in access.users):
                db.add_all(
                    [
                        models.Access(
                            user_id=user.id,
                            image_path=img_name,
                            success=access.success,
                        )
                        for user in access.users
                        if user is not None
                    ]
                )
            else:
                db.add(models.Access(image_path=img_name, success=access.success))
            db.commit()
        except Exception as e:
            print(e)
            os.remove(p)

    @staticmethod
    def delete_user(db: Session, user_id: int, force: bool) -> int:
        if not force:
            res = (
                db.query(models.User)
                .filter(models.User.id == user_id)
                .update({models.User.is_active: False})
            )
        else:
            res = db.query(models.User).filter(models.User.id == user_id).delete()

        cache_client.delete("encodings")

        db.commit()

        return res

    @staticmethod
    def ativate_user(db: Session, user_id: int) -> int:
        res = (
            db.query(models.User)
            .filter(models.User.id == user_id)
            .update({models.User.is_active: True})
        )
        db.commit()

        return res

    @staticmethod
    def get_accesses(
        db: Session,
        skip: int = 0,
        limit: int = 100,
        start_date: datetime.date = None,
        end_date: datetime.date = None,
    ):
        now = datetime.now()
        if start_date is None:
            start_date = datetime(now.year, now.month, now.day)

        if end_date is None:
            end_date = start_date + timedelta(days=1)

        return (
            db.query(models.Access)
            .order_by(models.Access.created_at)
            .filter(models.Access.created_at.between(start_date, end_date))
            .offset(skip)
            .limit(limit)
            .all()
        )

    @staticmethod
    def get_accesses_by_user(
        db: Session,
        user_id: int,
        skip: int = 0,
        limit: int = 100,
        start_date: datetime.date = None,
        end_date: datetime.date = None,
    ) -> List[schemas.AccessRead]:

        now = datetime.now()
        if start_date is None:
            start_date = datetime(now.year, now.month, now.day)

        if end_date is None:
            end_date = start_date + timedelta(days=1)

        return (
            db.query(models.Access)
            .filter(models.Access.user_id == user_id)
            .filter(models.Access.created_at.between(start_date, end_date))
            .order_by(models.Access.created_at)
            .offset(skip)
            .limit(limit)
            .all()
        )

    @staticmethod
    def __get_face_encodings__(db: Session) -> Dict:
        face_users = cache_client.get("encodings")

        if face_users is not None:
            return pickle.loads(face_users)

        query_result = (
            db.query(models.Encoding.data, models.User)
            .join(models.User)
            .filter(models.User.is_active == expression.true())
            .all()
        )
        face_users = {"encodings": [], "users": []}
        for (encoding, user) in query_result:
            face_users["encodings"].append(pickle.loads(encoding))
            face_users["users"].append(user)

        cache_client.set("encodings", pickle.dumps(face_users))
        return face_users
