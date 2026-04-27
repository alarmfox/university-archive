from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from app.db import models
from app.db.session import engine
from app.core.settings import get_settings
from app.api.users import router

settings = get_settings()

app = FastAPI()

app.include_router(router, prefix="/users", tags=["Users"])
app.mount(
    "/access", StaticFiles(directory=settings.access_log_directory), name="access"
)


@app.on_event("startup")
def startup():
    models.Base.metadata.create_all(bind=engine)
