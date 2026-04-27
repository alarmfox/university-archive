FROM continuumio/miniconda3 as build

WORKDIR /app

COPY environment.yml .

RUN conda init bash
RUN conda env create --file environment.yml -n sm


FROM python:3.9.13-slim-buster

WORKDIR /app

COPY --from=build /opt/conda/envs/sm /opt/conda/envs/sm
COPY . .

ENV PATH /opt/conda/envs/sm/bin:$PATH

EXPOSE 8000

CMD ["uvicorn", "app.main:app"]
