const { dbClient }= require('../config/dbConfig');
const User = require('../entities/User.js');

const getUserById = async (id) => {
  const result = await dbClient.query('SELECT id, nome FROM usuarios WHERE id = $1', [id]);
    
  if (result.rows.length > 0) {
    const { id, nome } = result.rows[0];
    return new User(id, nome);
  }
  };

  module.exports = { getUserById };