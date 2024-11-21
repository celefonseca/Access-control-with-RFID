const { dbClient }= require('../config/dbConfig');

const getUserById = async (id) => {
    const result = await dbClient.query('SELECT id FROM usuarios WHERE id = $1', [id]);
    return result.rows;
  };

  module.exports = { getUserById };