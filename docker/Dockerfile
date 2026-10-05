# Official Debian base image
FROM debian:bookworm-slim

LABEL maintainer="Megacoin Developers <info@megacoin.eu>"
LABEL description="Official Megacoin Daemon Docker image (x86_64)"

# Install runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    gosu \
    procps \
    && rm -rf /var/lib/apt/lists/*

# Create megacoin user and group with explicit UID/GID
RUN addgroup --gid 1000 megacoin \
    && adduser --disabled-password --gecos "" --uid 1000 --gid 1000 megacoin

# Set up data directory
ENV MEGACOIN_DATA=/var/lib/megacoin
RUN mkdir -p ${MEGACOIN_DATA} && chown -R megacoin:megacoin ${MEGACOIN_DATA}

# Copy x86_64 binaries into system PATH
COPY megacoin-release-1.10.0-linux-x86_64/megacoind /usr/local/bin/megacoind
COPY megacoin-release-1.10.0-linux-x86_64/megacoin-cli /usr/local/bin/megacoin-cli
COPY megacoin-release-1.10.0-linux-x86_64/megacoin-tx /usr/local/bin/megacoin-tx

RUN chmod +x /usr/local/bin/megacoind /usr/local/bin/megacoin-cli /usr/local/bin/megacoin-tx

# Copy entrypoint script
COPY docker/docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

# Expose Megacoin P2P port (7951) and RPC port (9469)
EXPOSE 7951 9469

VOLUME ["/var/lib/megacoin"]

ENTRYPOINT ["docker-entrypoint.sh"]

CMD ["megacoind", "-printtoconsole"]
