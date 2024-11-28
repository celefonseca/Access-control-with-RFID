# Usar uma imagem oficial do Node.js
FROM node:lts-alpine

# Definir o ambiente como produção
ENV NODE_ENV=production

# Definir o diretório de trabalho
WORKDIR /usr/src/app

# Copiar arquivos de dependências
COPY ["package.json", "package-lock.json*", "./"]

# Instalar as dependências
RUN npm install --production --silent

# Copiar o restante do código da aplicação
COPY . .

# Expor a porta usada pela aplicação
EXPOSE 3000

# Garantir permissões para o diretório de trabalho
RUN chown -R node /usr/src/app

# Definir o usuário para rodar o container
USER node

# Comando para rodar o servidor
CMD ["node", "src/index.js"]