from functools import lru_cache
from pydantic import BaseSettings


class Settings(BaseSettings):
    max_file_size: int = 3 * 1024**2
    database_url: str = "postgresql://127.0.0.1:5432/db"
    model: str = "hog"
    access_log_directory: str = "access_log"
    max_limit: int = 100
    cache_url: str = "localhost:6379"

    class Config:
        env_file = ".env"


@lru_cache
def get_settings() -> Settings:
    return Settings()
