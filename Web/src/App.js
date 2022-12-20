import logo from './logo.svg';
import './App.css';
import Navbar from './components/Navbar';
import Info from './features/Info';
import MyDatePicker from './MyDatePicker';
import Content from './components/Content';

function App() {
  return (
    <div className="App">
      <Navbar/>
      <Content/>
    </div>
  );
}

export default App;
