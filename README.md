# RDMA
# 一、 功能描述
## 企业内部管理系统
## 包含的角色模块
| 模块     | 系统管理员 | 部门经理 | 普通员工 |
| ------ | ----- | ---- | ---- |
| 用户管理   | ✅     | ✅    | ❌    |
| 部门管理   | ✅     | ❌    | ❌    |
| 个人中心   | ✅     | ✅    | ✅    |
| 新增/删用户 | ✅     | ❌    | ❌    |
| 获取用户   | ✅     | ✅    | ❌    |
| 修改密码   | ✅     | ✅    | ✅    |

# 二、 技术栈 React+Vite+Crow+MySQL
## 后端
### 数据库MySQL
 填充RBAC数据执行 create_and_init_rbac.sql
### 采用Crow框架
c++后端密码加密采用bcrypt ,克隆仓库

## 前端
### 采用React+Vite+Ts+tailwindcss
|软件|版本|
|:---|:---|
| nodejs | v20.19.2 |
| npm | v11.4.2 |
| tailwindcss | v4.1.10 |
# 运行
1. 前端：进入frontend文件夹，执行npm run dev
2. 后端：进入执行 ./build/crow_server
