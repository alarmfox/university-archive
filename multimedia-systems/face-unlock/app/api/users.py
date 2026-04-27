from datetime import date
import io
import os.path
import zipfile
from typing import List, Union

from sqlalchemy.orm import Session
from fastapi.responses import StreamingResponse
from fastapi import Depends, APIRouter, HTTPException, status, UploadFile, Form, Query

from app.db import schemas
from app.core.settings import get_settings
from app.db.session import SessionLocal
from app.core.image_processing import ImageProcessing
from app.service.users import UserService, DuplicateError
from app.api.utils import verify_files_opts, verify_file_opts, verify_limit


router = APIRouter()


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


settings = get_settings()


image_processor = ImageProcessing(model=settings.model)
user_service = UserService(image_processor, settings.access_log_directory)


@router.post(
    "/register",
    dependencies=[Depends(verify_files_opts)],
    response_class=StreamingResponse,
    status_code=status.HTTP_201_CREATED,
)
def create_user(
    files: List[UploadFile],
    db: Session = Depends(get_db),
    username: str = Form(),
    first_name: str = Form(),
    last_name: str = Form(),
    email: str = Form(),
):
    user = schemas.UserCreate(
        username=username, email=email, last_name=last_name, first_name=first_name
    )

    try:
        images = user_service.create_user(
            db=db, user=user, files=[file.file for file in files]
        )
    except DuplicateError:
        raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail={"message": "user already exists"},
        )

    with io.BytesIO() as buffer:
        with zipfile.ZipFile(buffer, mode="w", compression=zipfile.ZIP_DEFLATED) as zip:
            for i, image in enumerate(images):
                zip.writestr(f"{str(i)}.jpg", image)

        return StreamingResponse(
            iter([buffer.getvalue()]),
            media_type="application/x-zip-compressed",
            headers={"Content-Disposition": f"attachment;filename={username}.zip"},
        )


@router.post(
    "/login", dependencies=[Depends(verify_file_opts)], response_class=StreamingResponse
)
def login(file: UploadFile, db: Session = Depends(get_db)):
    try:
        img, users = user_service.login(db, file.file)
        if (
            users is None
            or len(users) == 0
            or not any(user is not None for user in users)
        ):
            user_service.register_access(
                db,
                schemas.AccessCreate(users=[], success=False),
                img,
            )
            raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED)
        user_service.register_access(
            db,
            schemas.AccessCreate(users=users, success=True),
            img,
        )
    except HTTPException:
        raise
    except Exception as ex:
        print(ex)
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail={"message": "something went wrong"},
        )

    fname = os.path.splitext(file.filename)[0]
    jpeg = image_processor.to_jpeg(img)
    return StreamingResponse(
        io.BytesIO(jpeg.tobytes()),
        media_type="image/jpeg",
        headers={"Content-Disposition": f"attachment; filename={fname}-tagged.jpg"},
    )


@router.get(
    "/", response_model=List[schemas.UserRead], dependencies=[Depends(verify_limit)]
)
def read_users(skip: int = 0, limit: int = 100, db: Session = Depends(get_db)):
    return UserService.get_users(db, skip=skip, limit=limit)


@router.get(
    "/accesses",
    response_model=list[schemas.AccessRead],
    dependencies=[Depends(verify_limit)],
)
def read_access(
    skip: int = 0,
    limit: int = 100,
    db: Session = Depends(get_db),
    start_date: Union[date, None] = Query(default=None),
    end_date: Union[date, None] = Query(default=None),
):
    return UserService.get_accesses(
        db, skip=skip, limit=limit, start_date=start_date, end_date=end_date
    )


@router.get(
    "/accesses/{user_id}",
    response_model=list[schemas.AccessRead],
    dependencies=[Depends(verify_limit)],
)
def read_access_by_user(
    user_id: int,
    start_date: Union[date, None] = Query(default=None),
    end_date: Union[date, None] = Query(default=None),
    skip: int = 0,
    limit: int = 100,
    db: Session = Depends(get_db),
):
    return UserService.get_accesses_by_user(
        db,
        user_id=user_id,
        skip=skip,
        limit=limit,
        start_date=start_date,
        end_date=end_date,
    )


@router.delete(
    "/{user_id}",
    status_code=status.HTTP_204_NO_CONTENT,
)
def delete_user(user_id: int, db: Session = Depends(get_db), force: bool = False):
    n = UserService.delete_user(db, user_id=user_id, force=force)
    if n == 0:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail={"message": "user not found"}
        )


@router.head(
    "/{user_id}",
    status_code=status.HTTP_204_NO_CONTENT,
)
def activate_user(user_id: int, db: Session = Depends(get_db)):
    n = UserService.ativate_user(db, user_id=user_id)
    if n == 0:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail={"message": "user not found"}
        )
