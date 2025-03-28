FROM node:18.19.1-alpine

WORKDIR /app

COPY . /app/

RUN npm i && npm run build

EXPOSE 8080

CMD [ "npm", "run", "api"]

