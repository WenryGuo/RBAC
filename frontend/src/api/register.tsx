import { useState, useEffect } from "react";
import axios from "axios";
import { useNavigate } from "react-router-dom";

function Register() {
  const [username, setUsername] = useState("");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [roles, setRoles] = useState<string[]>([]);
  const [selectedRole, setSelectedRole] = useState("");
  const [error, setError] = useState("");
  const navigate = useNavigate();

  // 获取角色列表
  useEffect(() => {
    axios
      .get("http://localhost:8080/roles")
      .then((res) => {
        if (res.data.roles) {
          setRoles(res.data.roles);
          setSelectedRole(res.data.roles[0]); // 默认选择第一个角色
        }
      })
      .catch((err) => {
        console.error("获取角色失败", err);
      });
  }, []);

  const handleRegister = async () => {
    try {
      const res = await axios.post("http://localhost:8080/register", {
        username,
        email,
        password,
        role: selectedRole, // 👈 添加角色字段
      });
      alert(res.data.message || "注册成功");
      navigate("/login");
    } catch (err: any) {
      console.error("注册失败:", err);
      setError(err.response?.data || "注册失败");
    }
  };

  return (
    <div className="flex flex-col items-center justify-center h-screen bg-gray-100">
      <h1 className="text-2xl font-bold mb-6">注册新用户</h1>
      <input
        type="text"
        placeholder="用户名"
        value={username}
        onChange={(e) => setUsername(e.target.value)}
        className="mb-2 p-2 border w-64 rounded"
      />
      <input
        type="email"
        placeholder="邮箱"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
        className="mb-2 p-2 border w-64 rounded"
      />
      <input
        type="password"
        placeholder="密码"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
        className="mb-2 p-2 border w-64 rounded"
      />

      {/* 👇 添加角色下拉框 */}
      <select
        value={selectedRole}
        onChange={(e) => setSelectedRole(e.target.value)}
        className="mb-4 p-2 border w-64 rounded"
      >
        {roles.map((role, index) => (
          <option key={index} value={role}>
            {role}
          </option>
        ))}
      </select>

      <button
        onClick={handleRegister}
        className="bg-green-500 hover:bg-green-600 text-white px-6 py-2 rounded"
      >
        注册
      </button>
      {error && <p className="text-red-500 mt-2">{error}</p>}
    </div>
  );
}

export default Register;
