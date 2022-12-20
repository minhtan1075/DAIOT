package com.example.demo.service;

import com.example.demo.model.CheckIn;

import java.util.List;

public interface CheckInService {

    List<CheckIn> getAllCheckInByDate(String date);

}
