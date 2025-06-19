-- 表结构定义
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    password_hash CHAR(60) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS roles (
    role_id INT AUTO_INCREMENT PRIMARY KEY COMMENT '角色唯一ID',
    role_name VARCHAR(50) NOT NULL COMMENT '角色名称',
    description TEXT COMMENT '角色描述',
    status TINYINT DEFAULT 1 NOT NULL COMMENT '状态（1启用/0禁用）'
);

CREATE TABLE IF NOT EXISTS modules (
    module_id INT AUTO_INCREMENT PRIMARY KEY COMMENT '模块唯一ID',
    module_name VARCHAR(50) NOT NULL COMMENT '模块名称',
    module_type TINYINT NOT NULL COMMENT '类型（1菜单/2按钮/3API）',
    parent_id INT DEFAULT 0 COMMENT '父模块ID（0为根节点）',
    url VARCHAR(255) COMMENT '访问路径（菜单路由或API地址）',
    KEY idx_parent_id (parent_id)
);

CREATE TABLE IF NOT EXISTS user_roles (
    user_id INT NOT NULL,
    role_id INT NOT NULL,
    PRIMARY KEY (user_id, role_id),
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (role_id) REFERENCES roles(role_id)
);

CREATE TABLE IF NOT EXISTS role_module (
    id INT AUTO_INCREMENT PRIMARY KEY,
    role_id INT NOT NULL COMMENT '角色ID',
    module_id INT NOT NULL COMMENT '模块ID',
    FOREIGN KEY (role_id) REFERENCES roles(role_id) ON DELETE CASCADE,
    FOREIGN KEY (module_id) REFERENCES modules(module_id) ON DELETE CASCADE,
    UNIQUE KEY uniq_role_module (role_id, module_id)
);

-- 初始化角色
INSERT INTO roles (role_name, description, status)
VALUES 
('系统管理员', '拥有系统全部权限', 1),
('部门经理', '管理部门资源和员工', 1),
('普通员工', '查看和提交自己的信息', 1);

-- 初始化用户（密码123的bcrypt哈希）
INSERT INTO users (username, email, password_hash)
VALUES 
('alice', 'alice@example.com', '$2y$10$kA2lD7t4h8ZqkUVsjX2XNe6tcqLvwKbc7RgpyLGZErxEVIGnk6G4a'),
('bob', 'bob@example.com', '$2y$10$kA2lD7t4h8ZqkUVsjX2XNe6tcqLvwKbc7RgpyLGZErxEVIGnk6G4a'),
('charlie', 'charlie@example.com', '$2y$10$kA2lD7t4h8ZqkUVsjX2XNe6tcqLvwKbc7RgpyLGZErxEVIGnk6G4a');

-- 绑定用户与角色
INSERT INTO user_roles (user_id, role_id)
VALUES
(1, 1), -- alice 是系统管理员
(2, 2), -- bob 是部门经理
(3, 3); -- charlie 是普通员工

-- 初始化模块
INSERT INTO modules (module_name, module_type, parent_id, url)
VALUES
('用户管理', 1, 0, '/users'),
('部门管理', 1, 0, '/departments'),
('个人中心', 1, 0, '/profile'),
('新增用户', 2, 1, NULL),
('删除用户', 2, 1, NULL),
('获取用户列表', 3, 0, '/api/users'),
('修改密码', 3, 0, '/api/change-password');

-- 角色与模块绑定
-- 系统管理员拥有所有模块权限
INSERT INTO role_module (role_id, module_id)
SELECT 1, module_id FROM modules;

-- 部门经理权限
INSERT INTO role_module (role_id, module_id)
SELECT 2, module_id FROM modules 
WHERE module_name IN ('用户管理', '获取用户列表');

-- 普通员工权限
INSERT INTO role_module (role_id, module_id)
SELECT 3, module_id FROM modules 
WHERE module_name IN ('个人中心', '修改密码');