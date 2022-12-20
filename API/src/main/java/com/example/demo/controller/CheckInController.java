package com.example.demo.controller;

import com.example.demo.service.CheckInService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@CrossOrigin
public class CheckInController {

    @Autowired
    private CheckInService checkInService;

    @GetMapping("/checkin")
    public ResponseEntity checkIn(@RequestParam String date) {
        return ResponseEntity.ok(checkInService.getAllCheckInByDate(date));
    }
}
