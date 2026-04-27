from app.db.session import Base
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func, expression
from sqlalchemy import (
    Column,
    ForeignKey,
    Integer,
    String,
    LargeBinary,
    TIMESTAMP,
    Boolean,
)


class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True, index=True)
    first_name = Column(String)
    last_name = Column(String)
    email = Column(String, unique=True)

    encodings = relationship("Encoding", back_populates="owner")
    accesses = relationship("Access", back_populates="user")

    is_active = Column(Boolean, unique=False, server_default=expression.false())


class Encoding(Base):
    __tablename__ = "encodings"

    id = Column(Integer, primary_key=True, index=True)
    data = Column(LargeBinary)
    owner_id = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"))

    owner = relationship("User", back_populates="encodings")


class Access(Base):
    __tablename__ = "accesses"

    id = Column(Integer, primary_key=True)

    image_path = Column(String)

    success = Column(Boolean)

    user_id = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"))

    user = relationship("User", back_populates="accesses")

    created_at = Column(TIMESTAMP, index=True, server_default=func.now())
