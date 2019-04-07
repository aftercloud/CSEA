package com.spring.eduspace.resources;

import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface ExerciseRepository extends JpaRepository<Exercise, Long> {
    Exercise findById(long id);

    @Override
    List<Exercise> findAll();
}
