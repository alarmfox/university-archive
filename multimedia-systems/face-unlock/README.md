# Progetto sistemi multimediali

## Descrizione
L'applicazione espone su endpoint HTTP le funzionalità per il riconoscimento facciale per poter sbloccare una serratura con controllo centralizzato. L'applicazione è pensata per essere agnostica ai dispositivi fisici (scanner, fotocamere ecc) ed effettua un matching dei volti in un immagine con quelli precendentemente registrati.
Inoltre, l'applicazione mette a disposizione le funzionalità per:
* gestione utenti: un utente amministratore può registrare o eliminare utenti;
* controllo accessi: per ogni accesso (sia che abbia avuto successo o meno), l'applicazione memorizza i volti riconosciuti e salva sul file system l'immagine con cui ha effettuato il riconoscimento.<br>
Il sistema è composta da un'applicazione Python che si comporta da server HTTP ed effettua il riconoscimento facciale, un database PostgreSQL e una cache Redis.

L'applicazione espone i seguenti endpoint:
* POST /users/login: data un'immagine in ingresso l'applicazione ritorna 200 se l'utente viene riconosciuto e allega l'immagine del match;
* POST /users/register: data una lista di immagini in ingresso e uno username l'applicazione registra i volti rilevati in quelle immagini come associati allo username in ingresso. Ritorna uno zip con le immagini taggate;
* GET /users: ritorna tutti gli username memorizzati nel sistema;
* DELETE /users/{user_id}?force={true|false}: elimina l'utente con l'id passato nell'URL. Se il flag force è *false*, allora l'utente viene disattivato e gli accessi restano restano salvati. Se è *True*, l'utente, insieme ad encoding e accessi, viene eliminato dal database;
* GET /users/accesses?start_date={start_date}&end_date={end_date}: ritorna tutti gli accessi nel sistema;
* GET /users/accesses/{user_id}?start_date={start_date}&end_date={end_date}: ritorna tutti gli accessi nel sistema per uno specifico utente;
* HEAD /users/{user_id}: riattiva un utente disattivato;

## Moduli utilizzati
Le librerie utilizzate sono:
* fastapi: modulo per la creazione di un servizio http;
* opencv: modulo per image processing;
* face_recognition: modulo per il riconoscimento facciale basato sulla libreria dlib;
* psycopg2: driver per la comunicazione con PostgreSQL;
* sql-alchemy: orm per mappare dati in classi;
* redis[hiredis]: libreria per comunicare con Redis;
## Configurazione
L'applicazione si configura attraverso variabili d'ambiente:
* MAX_FILE_SIZE: massima dimensione delle immagini in byte: Default "3145728" (3MB);
* DATABASE_URL: url per connettersi a un database PostgreSQL. L'url deve essere nella forma __postgresql://username:password@host:port/db?<parametri>__. Default "postgresql://127.0.0.1:5432/db";
* ACCESS_LOG_DIRECTORY: cartella in cui salvare le immagini relative agli accessi;
* MODEL: modello da utilizzare per estrarre i punti del volto. Può essere o "hog" oppure "cnn". Default "hog";
* MAX_LIMIT: massimo numero di record tornati in una sola risposta. Default "100";
* CACHE_URL: url di Redis. Default "localhost:6379";

Per configurare le variabili, può essere usato un file `.env` sulla base di `.env.example`.

## Installazione ed esecuzione

### Docker compose
Questa modalità di esecuzione richiede l'installazione di Docker e docker-compose. È possibile scaricarli [qui](https://www.docker.com/products/docker-desktop/).<br>
Per eseguire subito un'istanza dell'applicazione (server + database), basta eseguire il comando:

```sh
docker-compose up
```

Questo comando eseguirà la creazione dell'immagine Docker dell'applicazione e scaricherà quelle di PostgreSQL e di Redis.

Per testare gli endpoint, navigare su http://localhost:8000/docs.

### Bare metal
Prima di eseguire il server, è necessario eseguire assicurarsi che sono in esecuzione Redis e il database Postgresql. Ad esempio possono essere eseguiti attraverso 2 container Docker:

```sh
docker run -p 6379:6379 -d redis:alpine
docker run -e POSTGRES_PASSWORD=postgres -p 5432:5432 -d postgres:10.21-alpine3.16
```

Il progetto è gestito con __conda__. Per ricreare l'ambiente virtuale eseguire i seguenti comandi:

```sh
conda env create --file environment.yml -n <nome>
conda activate <nome>
```

Per eseguire l'applicazione, dopo aver attivato l'environment con conda, eseguire il server:

```sh
uvicorn app.main:app
```

Per testare gli endpoint, navigare su http://localhost:8000/docs.

