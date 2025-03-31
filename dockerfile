FROM node:18.19.1-alpine

WORKDIR /app

COPY package*.json /app/

RUN npm i 

COPY . /app/

RUN npm run build

USER node

CMD [ "npm", "run", "api"]

