// src/api/users.ts
// import axios from 'axios';

// const API_BASE = 'http://localhost:8080';

// export const fetchUsers = async () => {
//   const res = await axios.get(`${API_BASE}/users`);
//   return res.data;
// };
import axios from 'axios';

export const fetchUsers = async () => {
  const res = await axios.get('http://localhost:8080/json');
  return res.data.users || [];
};

