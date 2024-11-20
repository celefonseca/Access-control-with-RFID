# Usar uma imagem base oficial do Node.js
FROM node:18-alpine

# Definir o diretório de trabalho no container
WORKDIR /app

# Copiar arquivos do projeto para o container
COPY package*.json ./

# Instalar as dependências
RUN npm install

# Copiar o restante do código
COPY . .

# Expor a porta usada pela aplicação
EXPOSE 3000

# Comando para rodar a aplicação
CMD ["node", "src/index.js"]
