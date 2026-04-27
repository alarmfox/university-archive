from pydantic import BaseModel
from typing import List, Union

from datetime import datetime


class UserRead(BaseModel):
    id: int
    username: str
    first_name: str
    last_name: str
    email: str
    is_active: bool

    class Config:
        orm_mode = True


class UserCreate(BaseModel):
    username: str
    first_name: str
    last_name: str
    email: str

    class Config:
        orm_mode = True


class AccessCreate(BaseModel):
    users: List[Union[UserRead, None]]
    success: bool

    class Config:
        orm_mode = True


class AccessRead(BaseModel):
    id: int
    user_id: Union[int, None]
    success: bool
    image_path: str
    created_at: datetime

    class Config:
        orm_mode = True
