package com.example.demo.model;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class CheckIn {
    private String date;
    private String time;
    private String teacherId;
    private String firstName;
    private String lastName;
    private String phoneNumber;
    private String address;
}
