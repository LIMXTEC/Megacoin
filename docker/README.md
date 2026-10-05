# Megacoin Docker Deployment (Debian)

Contenedor Docker oficial basado en **Debian Bookworm (slim)** con los binarios x86_64 de Megacoin `v1.10.0` (`megacoind`, `megacoin-cli`, `megacoin-tx`).

Imagen pública en Docker Hub: [scotynau/megacoin](https://hub.docker.com/r/scotynau/megacoin)

---

## Despliegue Rápido desde Docker Hub

Para desplegar directamente sin compilar la imagen:

```bash
docker run -d \
  --name megacoin-node \
  -p 7951:7951 \
  -p 127.0.0.1:9469:9469 \
  -v megacoin-data:/var/lib/megacoin \
  scotynau/megacoin:latest
```

O usando `docker compose` desde esta carpeta:

```bash
docker compose up -d
```

---

## Construcción de la Imagen Local

Si deseas construir la imagen manualmente desde la raíz del repositorio:

```bash
docker build -t scotynau/megacoin:1.10.0 -f docker/Dockerfile .
```

---

## Ejecutar Comandos RPC (`megacoin-cli`)

Puedes consultar el estado del nodo directamente a través del contenedor:

```bash
docker exec -it megacoin-node megacoin-cli getblockchaininfo
docker exec -it megacoin-node megacoin-cli getnetworkinfo
```

---

## Configuración y Puertos

- **Puerto P2P:** `7951`
- **Puerto RPC:** `9469`
- **Volumen de Datos:** `/var/lib/megacoin`
