import redis

from app.core.settings import get_settings

host, port = get_settings().cache_url.split(':')

pool = redis.ConnectionPool(host=host, port=port, db=0)
client = redis.Redis(connection_pool=pool)
