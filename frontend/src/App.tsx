import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Home from "./pages/home";
import Register from './pages/register';
import Login from './pages/login';
import Users from './pages/users';

function App() {
  const [count, setCount] = useState(0)

  return (
    /* <>
       <div>
         <a href="https://vite.dev" target="_blank">
           <img src={viteLogo} className="logo" alt="Vite logo" />
         </a>
         <a href="https://react.dev" target="_blank">
           <img src={reactLogo} className="logo react" alt="React logo" />
           <div className="bg-green-500 text-white p-4">Tailwind 运行成功！</div>
         </a>
       </div>
       <h1>Vite + React</h1>
       <div className="card">
         <button onClick={() => setCount((count) => count + 1)}>
           count is {count}
         </button>
         <p>
           Edit <code>src/App.tsx</code> and save to test HMR
         </p>
       </div>
       <p className="read-the-docs">
         Click on the Vite and React logos to learn more
       </p>
     </>*/
    // <Router>
    //   <Routes>
    //     <Route path="/" element={<Users />} />
    //   </Routes>
    // </Router>
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/login" element={<Login />} />
      <Route path="/register" element={<Register />} />
      <Route path="/users" element={<Users />} />
    </Routes>
  )
}

export default App
