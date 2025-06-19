// src/pages/Users.tsx
import { useEffect, useState } from "react";
import axios from "axios";

function Users() {
  const [users, setUsers] = useState<string[]>([]);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    axios.get("http://localhost:8080/json")
      .then(res => {
        if (res.data.users) {
          const userList = Object.values(res.data.users);
          setUsers(userList as string[]);
        } else {
          setError("无用户数据");
        }
      })
      .catch(err => {
        console.error("获取用户失败:", err);
        setError("请求出错，请检查后端是否运行");
      });
  }, []);

  return (
    <div className="p-4">
      <h1 className="text-xl font-bold mb-4">用户列表</h1>
      {error && <p className="text-red-500">{error}</p>}
      <ul className="list-disc pl-6">
        {users.map((u, i) => (
          <li key={i}>{u}</li>
        ))}
      </ul>
    </div>
  );
}

export default Users;


