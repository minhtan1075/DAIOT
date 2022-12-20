import React from 'react';
import PropTypes from 'prop-types';
import './styles.scss';
import Info from '../../features/Info';
import MyDatePicker from '../../MyDatePicker';

Content.propTypes = {
    
};

function Content(props) {
    return (
        <div className='d1'>
            <MyDatePicker/>
        </div>
    );
}

export default Content;