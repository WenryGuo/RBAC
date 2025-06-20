import { useNavigate } from "react-router-dom";

function Home() {
  const navigate = useNavigate();

  return (
    <div className="flex flex-col items-center justify-center h-screen bg-gray-100">
      <h1 className="text-3xl font-bold mb-6">欢迎使用 RBAC 系统</h1>
      <div className="space-x-4">
        <button
          onClick={() => navigate("/login")}
          className="bg-blue-500 hover:bg-blue-600 text-white px-6 py-2 rounded"
        >
          登录
        </button>
        <button
          onClick={() => navigate("/register")}
          className="bg-green-500 hover:bg-green-600 text-white px-6 py-2 rounded"
        >
          注册
        </button>
      </div>
    </div>
  );
}

export default Home;
