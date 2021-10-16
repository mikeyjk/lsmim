FROM debian:bookworm as build
RUN apt update && \
  apt install -y \
    curl \
    bzip2 \
    gcc \ 
    make
WORKDIR /app
RUN mkdir criterion
RUN export CRITERION_VERSION=2.3.3 && \
  curl -L \
  https://github.com/Snaipe/Criterion/releases/download/v${CRITERION_VERSION}/criterion-v${CRITERION_VERSION}-linux-x86_64.tar.bz2 \
    --output criterion.tar.bz2
RUN bzip2 -d criterion.tar.bz2 && \
  tar -xvf criterion.tar -C criterion --strip-components=1
RUN cp -prf criterion/lib/** /usr/lib && \
  cp -prf criterion/lib /lib
COPY *.c Makefile ./
RUN mkdir bin && make

FROM debian:bookworm
RUN mkdir -p /app/bin
COPY --from=build /app/bin/lsmim /app/bin/lsmim
RUN ls -al /app/ /app/bin/ /app/bin/lsmim
RUN /app/bin/lsmim
ENTRYPOINT [ "/app/bin/lsmim", "-s" ]
