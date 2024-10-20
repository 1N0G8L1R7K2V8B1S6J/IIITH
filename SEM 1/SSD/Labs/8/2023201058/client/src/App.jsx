import React from 'react';
import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Login from "./Login"
import Signup from "./Signup"
import Dashboard from './Dashboard'
import Home from './Home'
import Admin from "./Admin"
import CompleteToDo from './CompleteToDo';
import PendingToDo from './PendingToDo';

function App() {

    return (
        <BrowserRouter>
            <Routes>
                <Route path="/" element={<Home />}></Route>
                <Route path="/register" element={<Signup />}></Route>
                <Route path="/login" element={<Login />}></Route>
                <Route path="/dashboard" element={<Dashboard />}></Route>
                <Route path="/admin" element={<Admin />}></Route>
                <Route path="/complete-todo" element={<CompleteToDo />}></Route>
                <Route path="/pending-todo" element={<PendingToDo />}></Route>
            </Routes>
        </BrowserRouter>
    )
}

export default App



