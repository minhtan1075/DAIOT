import * as React from 'react';
import PropTypes from 'prop-types';
import './styles.scss';
import MyDatePicker from '../../MyDatePicker';

Info.propTypes = {
    
};

function Info(props) {
    const {list} = props;
    return (
        <div className='margin'>
            <table>
                <tr>
                    <th>Ngày</th>
                    <th>Thời gian</th>
                    <th>Id</th>
                    <th>Họ</th>
                    <th>Tên</th>
                    <th>Sđt</th>
                    <th>Địa chỉ</th>
                </tr>
            
            {list.map((list , index) => <tr key={index} >
                <td>{list.date}</td>
                <td>{list.time}</td>
                <td>{list.teacherId}</td>
                <td>{list.firstName}</td>
                <td>{list.lastName}</td>
                <td>{list.phoneNumber}</td>
                <td>{list.address}</td>
            </tr>) 
            } 
            </table>
        </div>
    );
}

export default Info;