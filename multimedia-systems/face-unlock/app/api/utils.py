from typing import List

from fastapi import UploadFile, Request, HTTPException, status

from app.core.settings import get_settings

settings = get_settings()

accepted_content_types = ["image/jpeg"]
suffixes = ["B", "KB", "MB", "GB", "TB", "PB"]


def humansize(n_bytes: int) -> str:
    i = 0
    while n_bytes >= 1024 and i < len(suffixes) - 1:
        n_bytes /= 1024.0
        i += 1
    f = f"{n_bytes}".rstrip("0").rstrip(".")
    return f"{f} {suffixes[i]}"


async def verify_files_opts(request: Request, files: List[UploadFile]):
    if any(file.content_type not in accepted_content_types for file in files):
        raise HTTPException(
            status_code=status.HTTP_415_UNSUPPORTED_MEDIA_TYPE,
            detail={
                "message": f"accepted content type: {','.join(accepted_content_types)}"
            },
        )

    if "content-length" not in request.headers:
        raise HTTPException(status_code=status.HTTP_411_LENGTH_REQUIRED)

    if int(request.headers.get("content-length", 0)) > settings.max_file_size:
        raise HTTPException(
            status_code=status.HTTP_413_REQUEST_ENTITY_TOO_LARGE,
            detail={
                "message": f"max file allowed is {humansize(settings.max_file_size)}"
            },
        )


async def verify_file_opts(file: UploadFile, request: Request):
    if file.content_type not in accepted_content_types:
        raise HTTPException(
            status_code=status.HTTP_415_UNSUPPORTED_MEDIA_TYPE,
            detail={
                "message": f"accepted content type: {','.join(accepted_content_types)}"
            },
        )

    if "content-length" not in request.headers:
        raise HTTPException(status_code=status.HTTP_411_LENGTH_REQUIRED)

    if int(request.headers.get("content-length", 0)) > settings.max_file_size:
        raise HTTPException(
            status_code=status.HTTP_413_REQUEST_ENTITY_TOO_LARGE,
            detail={
                "message": f"max file allowed is {humansize(settings.max_file_size)}"
            },
        )


async def verify_limit(limit: int = 100):
    if limit > settings.max_limit:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST, detail={
            "message": f"max limit is {settings.max_limit}"
        })
