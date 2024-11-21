const { Client } = require('pg');
require('dotenv').config();

const dbClient = new Client({
  connectionString: process.env.DATABASE_URL,
});

// Usando async/await para garantir que a conexão seja estabelecida corretamente
const connectDb = async () => {
  try {
    await dbClient.connect();
    console.log('Conectado ao banco de dados PostgreSQL');
  } catch (err) {
    console.error('Erro ao conectar ao banco de dados:', err);
  }
};

connectDb();

module.exports = { dbClient };