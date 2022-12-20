import * as React from 'react';
import dayjs from 'dayjs';
import TextField from '@mui/material/TextField';
import { AdapterDayjs } from '@mui/x-date-pickers/AdapterDayjs';
import { LocalizationProvider } from '@mui/x-date-pickers/LocalizationProvider';
import { DesktopDatePicker } from '@mui/x-date-pickers/DesktopDatePicker';
import Info from '../features/Info';
import { FormControl } from '@mui/material';
import MenuItem from '@mui/material/MenuItem';
import InputLabel from '@mui/material/InputLabel';
import Select from '@mui/material/Select';
import attendanceApi from '../api/attendanceApi';

export default function MyDatePicker() {
  const [value, setValue] = React.useState(dayjs(Date()));
  const [id,setId] =React.useState(`none`);
  const [idValues,setIdValues] = React.useState([]);
const handleChange = (event) => {
    setId(event.target.value);
  };
  const [list,setList] = React.useState([])
  React.useEffect(() =>  {
      const fetchInfo = async () =>{ 
      try {
          const infoList = await attendanceApi.getall(value.format("YYYY-MM-DD"));
          if(id === `none`){
            setList(infoList);
          }else{
            setList(infoList.filter(value => value.teacherId === `${id}`))
          }
          
      } catch (error) {
          console.log('không lấy được dữ liệu', error);
      }
  }
  fetchInfo();
},[value,id]);
const allIdValues = [... new Set(list.map((iValue)=> iValue.teacherId))];
  return (
    <>
    <LocalizationProvider dateAdapter={AdapterDayjs}>
      
        <DesktopDatePicker
          label="chọn ngày"
          value={value}
          minDate={dayjs('2017-01-01')
        }
          onChange={(newValue) => {
            setValue(newValue);
          }}
          renderInput={(params) => <TextField {...params} />}
        />

        
    </LocalizationProvider>
    <FormControl sx={{width: 60}}>
      <InputLabel id="demo-simple-select-label">Id</InputLabel>
       <Select
          labelId="demo-simple-select-label"
          id="demo-simple-select"
          value={id}
          label="Id"
          onChange={handleChange}
        >
          <MenuItem value={`none`}><p></p></MenuItem>
          {
            allIdValues.map((value) => <MenuItem key = {value} value={value}>{value}</MenuItem>)
          }
        </Select>
    </FormControl>
    <Info list = {list}/>
    </>

  );
}
