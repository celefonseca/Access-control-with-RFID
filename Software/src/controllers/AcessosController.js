const Acessos = require('../entities/Acessos');
const AcessosRepository = require('../repositories/AcessosRepository');

class AcessosController {
  static async registerAcess(usuarioId) {
    try {
      // Cria uma nova instância da entidade Acessos
      const novoAcesso = new Acessos(usuarioId, new Date().toISOString());

      // Salva o acesso no banco de dados
      const acessoSalvo = await AcessosRepository.save(novoAcesso);

      console.log('Acesso registrado com sucesso:', acessoSalvo);
    } catch (error) {
      console.error('Erro ao registrar acesso:', error);
      throw error;
    }
  }

  static async listAccess() {
    try {
    const acessosComUsuarios = await AcessosRepository.listAccess();
    return acessosComUsuarios
  } catch (error) {
    console.error('Erro ao listar acessos:', error);
    throw error;
  }
  }
}

module.exports = AcessosController;