package com.example.demo.service;
import com.example.demo.model.CheckIn;
import com.google.api.client.googleapis.javanet.GoogleNetHttpTransport;
import com.google.api.client.json.JsonFactory;
import com.google.api.client.json.gson.GsonFactory;
import com.google.api.services.sheets.v4.Sheets;
import com.google.api.services.sheets.v4.SheetsScopes;
import lombok.SneakyThrows;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.time.chrono.ChronoLocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeFormatterBuilder;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.stream.Collectors;

import static com.example.demo.utils.SheetsQuickstart.getCredentials;
import static java.time.LocalDate.parse;
import static java.util.Collections.EMPTY_LIST;
import static java.util.Collections.singletonList;

@Service
@Slf4j
public class CheckInServiceImpl implements CheckInService{

    private static final String APPLICATION_NAME = "CheckInApp";
    private static final JsonFactory JSON_FACTORY = GsonFactory.getDefaultInstance();
    public static final String SPREADSHEET_ID = "1MVweXsQ1r6gvLC4UEtjK2rzPgLSBPUaoIweG8MhZeTg";
    public static final String RANGE = "A:G";

    @Override
    public List<CheckIn> getAllCheckInByDate(String date) {
        var checkInList = getCheckInList();
        if (checkInList.isEmpty()) {
            log.debug("Empty list");
            return EMPTY_LIST;
        }
        log.debug("list size: " + checkInList.size());
        return checkInList.stream()
                .filter(x -> x.getDate().equalsIgnoreCase(date))
                .collect(Collectors.toList());
    }

    @SneakyThrows
    private List<CheckIn> getCheckInList() {
        final var HTTP_TRANSPORT = GoogleNetHttpTransport.newTrustedTransport();
        var service = new Sheets.Builder(HTTP_TRANSPORT, JSON_FACTORY, getCredentials(HTTP_TRANSPORT))
                        .setApplicationName(APPLICATION_NAME)
                        .build();
        var response = service.spreadsheets().values()
                .get(SPREADSHEET_ID, RANGE)
                .execute();
        var values = response.getValues();
        if (values == null || values.isEmpty()) {
            log.warn("No data found.");
            return EMPTY_LIST;
        } else {
            var checkInList = values.stream().skip(1).map(row ->CheckIn.builder()
                    .date((String) row.get(0))
                    .time((String) row.get(1))
                    .teacherId(Objects.isNull(row.get(2)) ? "" : (String) row.get(2))
                    .firstName(Objects.isNull(row.get(3)) ? "" : (String) row.get(3))
                    .lastName(Objects.isNull(row.get(4)) ? "" : (String) row.get(4))
                    .phoneNumber(Objects.isNull(row.get(5)) ? "" : (String) row.get(5))
                    .address(Objects.isNull(row.get(6)) ? "" : (String) row.get(6))
                    .build())
                    .collect(Collectors.toList());
            checkInList.forEach(System.out::println);
            return checkInList;
        }
    }
}
