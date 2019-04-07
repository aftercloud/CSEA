package com.spring.eduspace.resources;

import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface HomeworkRepository extends JpaRepository<Homework, Long> {
    Homework findById(long id);

    @Override
    List<Homework> findAll();
}
