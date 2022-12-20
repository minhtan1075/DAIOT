import React from 'react';
import PropTypes from 'prop-types';
import './styles.scss';
Navbar.propTypes = {
    
};

function Navbar(props) {
    return (
        <div>
                <div className='Nav' >
                    <a>trang chủ</a>
                    <a className='point'>giáo viên
                    <div className="subNav">
                <a>thông tin</a>
                <a>tạo mới</a>
            </div></a>
                    <a>thông tin</a>
                    <a>giới thiệu</a>
                </div>
            
            
        </div>
    );
}

export default Navbar;