FROM gcc:12.2.0

WORKDIR /app

COPY src /app/src
COPY www /app/www

RUN g++ -O2 -std=gnu++17 -pthread /app/src/*.cpp -o /app/app

EXPOSE 8080

CMD ["/app/app"]
