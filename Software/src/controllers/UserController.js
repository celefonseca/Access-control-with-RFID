const {getUserById, saveUser} = require('../repositories/UserRepository.js');


const validateUser = async (userId) => {
    try {
      const user = await getUserById(userId);
      return user;
    } catch (err) {
      console.error('Erro ao validar usuario:', err);
      throw err;
    }
  }

  const registerUser = async (user) => {
    try {
      const { id, name } = user;
  
      if (!id || !name) {
        throw new Error('ID e nome são obrigatórios.');
      }
  
      const existingUser = await getUserById(id);
      if (existingUser) {
        throw new Error('Usuário já existe.');
      }
  
      const savedUser = await saveUser(user);
      console.log('Usuário salvo com sucesso:', savedUser);
      return savedUser;
    } catch (err) {
      console.error('Erro ao salvar usuário:', err);
      throw err;
    }
  };

  module.exports={validateUser, registerUser};