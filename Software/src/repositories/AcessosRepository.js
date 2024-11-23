const { dbClient } = require('../config/dbConfig');

class AcessosRepository {
  static async save(acesso) {
    const query = `
      INSERT INTO acessos (usuario_id, data_hora)
      VALUES ($1, $2)
      RETURNING *;
    `;
    const values = [acesso.usuarioId, acesso.dataHora];

    try {
      const result = await dbClient.query(query, values);
      return result.rows[0];
    } catch (error) {
      console.error('Erro ao salvar acesso no banco:', error);
      throw error;
    }
  }
}

module.exports = AcessosRepository;