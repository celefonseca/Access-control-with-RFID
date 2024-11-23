const UserRepository = require('../repositories/UserRepository.js');


const validateUser = async (userId) => {
    try {
      const user = await getUserById(userId);
      return user;
    } catch (err) {
      console.error('Erro ao validar usuario:', err);
      throw err;
    }
  }

  module.exports={validateUser};