import { useState } from "react";
import axios from "axios";
import { useNavigate } from "react-router-dom";

function Login() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState<string | null>(null);
  const navigate = useNavigate();

  const handleLogin = async () => {
    try {
      const res = await axios.post("http://localhost:8080/login", {
        username,
        password,
      });
      // 👇 登录成功，保存 role_id 和 user_id
      localStorage.setItem("user_id", res.data.user_id);
      localStorage.setItem("role_id", res.data.role_id);
      navigate("/users"); // 登录成功跳转到用户管理
    } catch (err: any) {
      setError(err.response?.data || "登录失败");
    }
  };

  return (
    <div className="p-4 max-w-sm mx-auto">
      <h1 className="text-xl font-bold mb-4">登录</h1>
      <input
        className="border p-2 w-full mb-2"
        placeholder="用户名"
        value={username}
        onChange={(e) => setUsername(e.target.value)}
      />
      <input
        className="border p-2 w-full mb-2"
        placeholder="密码"
        type="password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />
      <button className="bg-blue-500 text-white px-4 py-2" onClick={handleLogin}>
        登录
      </button>
      {error && <p className="text-red-500 mt-2">{error}</p>}
    </div>
  );
}

export default Login;
