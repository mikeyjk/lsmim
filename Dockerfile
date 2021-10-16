FROM debian:bookworm as build
RUN apt update && \
  apt install -y \
    curl \
    bzip2 \
    gcc \ 
    make
WORKDIR /app
RUN mkdir criterion
RUN curl -L \
  https://github.com/Snaipe/Criterion/releases/download/v2.3.3/criterion-v2.3.3-linux-x86_64.tar.bz2 \
  --output criterion.tar.bz2
RUN bzip2 -d criterion.tar.bz2
RUN tar -xvf criterion.tar -C criterion --strip-components=1
RUN cp -prf criterion/lib/** /usr/lib
RUN cp -prf criterion/lib /lib
COPY *.c Makefile ./
RUN mkdir bin && make

FROM debian:bookworm
RUN mkdir -p /app/bin
COPY --from=build /app/bin/lsmim /app/bin/lsmim
RUN ls -al /app/ /app/bin/ /app/bin/lsmim
RUN /app/bin/lsmim
ENTRYPOINT [ "/app/bin/lsmim" ]
