import { useEffect, useState } from "react";
import axios from "axios";

interface Role {
  id: number;
  name: string;
}

function Dashboard() {
  const [users, setUsers] = useState<string[]>([]);
  const [roleId, setRoleId] = useState<number>(0);
  const [error, setError] = useState<string | null>(null);
  const [roles, setRoles] = useState<Role[]>([]); // 👈 改为 Role 类型数组

  useEffect(() => {
    // 获取角色 ID
    const storedRoleId = parseInt(localStorage.getItem("role_id") || "0");
    setRoleId(storedRoleId);

    if (storedRoleId === 1) {
      // 管理员：获取用户列表
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

      // 管理员：获取角色列表
      axios.get("http://localhost:8080/roles")
        .then(res => {
          if (res.data.roles) {
            setRoles(res.data.roles); // 👈 应为对象数组，如 [{id: 1, name: "系统管理员"}]
          }
        })
        .catch((err) => {
          console.error("获取角色失败", err);
        });
    }
  }, []);

  return (
    <div className="p-6">
      <h1 className="text-2xl font-bold mb-4">RBAC 管理系统</h1>

      {roleId === 1 && (
        <div>
          <h2 className="text-xl font-semibold mb-2">系统管理员页面</h2>

          {/* 用户列表 */}
          <h2 className="text-xl font-semibold mb-2">用户列表</h2>
          {error && <p className="text-red-500">{error}</p>}
          <ul className="list-disc pl-6 mb-4">
            {users.map((u, i) => (
              <li key={i}>{u}</li>
            ))}
          </ul>

          <div className="flex gap-2 mb-6">
            <button className="bg-blue-500 hover:bg-blue-600 text-white px-4 py-2 rounded">
              添加用户
            </button>
            <button className="bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded">
              删除用户
            </button>
          </div>

          {/* 角色列表 */}
          <h2 className="text-xl font-semibold mb-2">角色列表</h2>
          <ul className="list-disc pl-6 mb-4">
            {roles.map((r) => (
              <li key={r.id}>
                <span className="font-medium">{r.name}</span> (ID: {r.id})
              </li>
            ))}
          </ul>

          <div className="flex gap-2">
            <button className="bg-blue-500 hover:bg-blue-600 text-white px-4 py-2 rounded">
              添加角色
            </button>
            <button className="bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded">
              删除角色
            </button>
          </div>
        </div>
      )}

      {roleId === 2 && (
        <div>
          <h2 className="text-xl font-semibold mb-2">部门管理员页面</h2>
          <button className="bg-green-500 hover:bg-green-600 text-white px-6 py-2 rounded">
            发布任务
          </button>
        </div>
      )}

      {roleId === 3 && (
        <div>
          <h2 className="text-xl font-semibold mb-2">员工页面</h2>
          <h2 className="text-xl font-semibold mb-2">任务列表</h2>
          <ul className="list-disc pl-6">
            <li>任务 A</li>
            <li>任务 B</li>
            <li>任务 C</li>
          </ul>
        </div>
      )}
    </div>
  );
}

export default Dashboard;
