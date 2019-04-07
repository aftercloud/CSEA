package com.spring.eduspace.resources;

import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface PdfInfoRepository extends JpaRepository<PdfInfo, Long> {
    PdfInfo findById(long id);

    @Override
    List<PdfInfo> findAll();
}
