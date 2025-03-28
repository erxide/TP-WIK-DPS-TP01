FROM node:18.19.1-alpine

WORKDIR /app

COPY . /app/

RUN npm i && npm run build

CMD [ "npm", "run", "api"]

