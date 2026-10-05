# Megacoin Docker Deployment (Debian)

Contenedor Docker oficial basado en **Debian Bookworm (slim)** con los binarios x86_64 de Megacoin `v1.10.0` (`megacoind`, `megacoin-cli`, `megacoin-tx`).

Imagen pública en Docker Hub: [scotynau/megacoin](https://hub.docker.com/r/scotynau/megacoin)

---

## Despliegue Rápido desde Docker Hub

Para desplegar directamente mapeando una carpeta local (`./data`) para acceder a la blockchain y archivos de configuración (`megacoin.conf`, `wallet.dat`):

```bash
docker run -d \
  --name megacoin-node \
  -p 7951:7951 \
  -p 127.0.0.1:9469:9469 \
  -v $(pwd)/data:/var/lib/megacoin \
  scotynau/megacoin:latest
```

O usando `docker compose` desde esta carpeta:

```bash
docker compose up -d
```

---

## Acceso y Edición de Archivos (`megacoin.conf`, `wallet.dat`)

El contenedor almacena los datos en `/var/lib/megacoin` dentro del sistema de archivos del contenedor.

### Opción 1: Mapeo de volumen local (Recomendado)
Al usar `-v $(pwd)/data:/var/lib/megacoin` o `docker compose`, se creará la carpeta `./data` en tu servidor o computadora.
Dentro de `./data` encontrarás:
- `megacoin.conf` (archivo de configuración)
- `wallet.dat` (cartera)
- `blocks/` y `chainstate/` (datos de la blockchain)

Puedes editar `megacoin.conf` o gestionar `wallet.dat` directamente desde tu sistema operativo con cualquier editor de texto (`nano ./data/megacoin.conf`, `vim`, etc.) y luego reiniciar el contenedor:
```bash
docker restart megacoin-node
```

### Opción 2: Copiar archivos desde/hacia el contenedor
Si no mapeaste una carpeta del host, puedes copiar archivos directamente:
- **Extraer `megacoin.conf`:**
  ```bash
  docker cp megacoin-node:/var/lib/megacoin/megacoin.conf ./megacoin.conf
  ```
- **Copiar de vuelta tras editar:**
  ```bash
  docker cp ./megacoin.conf megacoin-node:/var/lib/megacoin/megacoin.conf
  docker restart megacoin-node
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
- **Ruta de Datos en Contenedor:** `/var/lib/megacoin`

