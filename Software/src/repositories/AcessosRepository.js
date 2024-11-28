const { dbClient } = require('../config/dbConfig');

class AcessosRepository {
  static async save(acesso) {
    const query = `
      INSERT INTO acessos (usuario_id, evento_timestamp)
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

  static async listAccess() {
    try {
      // Consulta SQL com JOIN
      const query = `
        SELECT 
          acessos.id AS acesso_id, 
          acessos.usuario_id, 
          acessos.evento_timestamp, 
          usuarios.nome AS usuario_nome 
        FROM acessos
        INNER JOIN usuarios ON acessos.usuario_id = usuarios.id
      `;
  
      // Executa a consulta no banco de dados
      const result = await dbClient.query(query);
      
      // Processa os resultados retornados do banco de dados
      const acessosComUsuarios = result.rows.map((row) => ({
        id: row.acesso_id,
        usuarioId: row.usuario_id,
        dataHora: row.evento_timestamp,
        usuario: {
          id: row.usuario_id,
          nome: row.usuario_nome,
        },
      }));
  
      return acessosComUsuarios;
    } catch (error) {
      // Loga o erro e propaga para quem chamou a função
      console.error('Erro ao listar acessos com usuários:', error);
      throw error;
    }
  }
}

module.exports = AcessosRepository;