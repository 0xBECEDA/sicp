(defparameter *input* "input.txt")
(defparameter *output* "output.txt")


;; загрузка арифметических выражений в входной файл
(defun load-arithmetic-tests()
  (with-open-file (input_stream *input* :direction :output
                                :if-exists :supersede)
    (format input_stream "(+ 1 2) ~%")
    (format input_stream "(+ 1 (* 2 4))~%")
    (format input_stream "(- 9 1)~%")
    (format input_stream "(+ (* 2 4) (- 4 6))~%")
    (format input_stream "(+ (* 2 3 5 6 4) (- 4 6 1))~%")
    (format input_stream "(/ 2 0 )~%")
    (format input_stream "(/ 2 'd )~%")
    (format input_stream "(/ 2 4 )~%")
    (format input_stream "(/ 6 4 )~%")
    (format input_stream "(quite)~%")))

;; загрузка выражений-определений в входной файл
(defun load-definitions-tests()
  (with-open-file (input_stream *input* :direction :output
                                :if-exists :supersede)
    (format input_stream "(define n 2)~%")
    (format input_stream "n~%")
    (format input_stream "(define a '(1 2 3 4))~%")
    (format input_stream "(quite)~%")))

;; загрузка выражений-присваиваний в входной файл
(defun load-assignments-tests()
  (with-open-file (input_stream *input* :direction :output
                                :if-exists :supersede)
    (format input_stream "(define n 2)~%")
    (format input_stream "(set! n (+ 1 2)) ~%")
    (format input_stream "n~%")
    (format input_stream "(set! n (lambda (x) (+ x 1))) ~%")
    (format input_stream "(n 1) ~%")
    (format input_stream "(quite)~%")))

;; загрузка объявлений и вызовов функций в входной файл
(defun load-functions-tests()
  (with-open-file (input_stream *input* :direction :output
                                :if-exists :supersede)
    (format input_stream "(define (my-reverse lst)
                             (define (my-reverse lst new-lst)
                                 (if (null? lst)
                                     new-lst
                                     (my-reverse (cdr lst) (cons (car lst) new-lst))))
                           (my-reverse lst '()))~%")
    (format input_stream "(my-reverse '(1 2 3 4)) ~%")
    (format input_stream "(my-reverse '(1 (2 3) 4)) ~%")
    (format input_stream "(my-reverse '(1 (2 3) 4) 'd) ~%")
    (format input_stream "(my-reverse \"d\") ~%")

    (format input_stream "(define (factorial n)
                             (if (= n 0)
                                 1
                                 (* n (factorial (- n 1))))) ~%")
    (format input_stream "(factorial 3) ~%")
    (format input_stream "(factorial 6) ~%")

    (format input_stream "(define (fib n)
                             (cond ((= n 0) 0)
                                   ((= n 1) 1)
                                   (else (+ (fib (- n 1))
                                            (fib (- n 2)))))) ~%")
    (format input_stream "(fib 8) ~%")
    (format input_stream "(quite)~%")))


;; (load-assignments-tests)
;; (load-definitions-tests)
;; (load-arithmetic-tests)
;; (load-functions-tests)

;; исполняет арифметические тесты
(defun execute-arithmetic-tests()
  (load-arithmetic-tests)
  (with-open-file (input_stream *input* :direction :input)
    (with-open-file (output_stream *output* :direction :output
                                   :if-exists :supersede)
      (sb-ext:run-program "./proga" (list '()) :wait t :input input_stream
                          :output output_stream)
      (force-output output_stream)))
  (with-open-file (output_stream *output* :direction :input)
    (with-open-file (input_stream *input* :direction :input)
      (do ((output-line (read-line output_stream nil 'eof)
                        (read-line output_stream nil 'eof))
           (input-line (read-line output_stream nil 'eof)
                       (read-line output_stream nil 'eof)))
          ((eql input-line 'eof))
        (cond ((string= input-line "(+ 1 2)")
               (assert (string= output-line "3")))
              ((string= input-line "(+ 1 (* 2 4))")
               (assert (string= output-line "9")))
              ((string= input-line "(- 9 1)")
               (assert (string= output-line "8")))
              ((string= input-line "(+ (* 2 4) (- 4 6))")
               (assert (string= output-line "6")))
              ((string= input-line "(+ (* 2 3 5 6 4) (- 4 6 1))~%")
               (assert (string= output-line"717")))
              ((string= input-line "(/ 2 0 )")
               (assert (string= output-line "(DIVISION ERROR: division by zero)")))
              ((string= input-line "(/ 2 'd )")
               (assert (string= output-line
                                "(DIVISION ERROR: division gets only numbers)")))
              ((string= input-line "(/ 2 4 )")
               (assert (string= output-line
                                "(DIVISION ERROR: division does't support fractional numbers)")))
              ((string= input-line "(/ 6 4 )") (assert (string= output-line "1")))
              (t 'done)))))
  'done)

;; (execute-arithmetic-tests)

;; исполняет тесты для определений
(defun execute-definitions-tests()
  (load-definitions-tests)
  (with-open-file (input_stream *input* :direction :input)
    (with-open-file (output_stream *output* :direction :output
                                   :if-exists :supersede)
      (sb-ext:run-program "./proga" (list '()) :wait t :input input_stream
                          :output output_stream)
      (force-output output_stream)))
  (with-open-file (output_stream *output* :direction :input)
    (with-open-file (input_stream *input* :direction :input)
      (do ((output-line (read-line output_stream nil 'eof)
                        (read-line output_stream nil 'eof))
           (input-line (read-line input_stream nil 'eof)
                       (read-line input_stream nil 'eof)))
          ((eql input-line 'eof))
        (cond ((string= input-line "(define n 2)")
               (assert (string= output-line "ok")))
              ((string= input-line "n")
               (assert (string= output-line "2")))
              ((string= input-line "(define a '(1 2 3 4))")
               (assert (string= output-line "ok")))
              (t 'done)))))
  'done)

;; (execute-definitions-tests)

;; исполняет тесты для присваиваний
(defun execute-assignments-tests()
  (load-assignments-tests)
  (with-open-file (input_stream *input* :direction :input)
    (with-open-file (output_stream *output* :direction :output
                                   :if-exists :supersede)
      (sb-ext:run-program "./proga" (list '()) :wait t :input input_stream
                          :output output_stream)
      (force-output output_stream)))
  (with-open-file (output_stream *output* :direction :input)
    (with-open-file (input_stream *input* :direction :input)
      (do ((output-line (read-line output_stream nil 'eof)
                        (read-line output_stream nil 'eof))
           (input-line (read-line input_stream nil 'eof)
                       (read-line input_stream nil 'eof)))
          ((eql input-line 'eof))
        (cond ((string= input-line "(define n 2)")
               (assert (string= output-line "ok")))
              ((string= input-line "(set! n (+ 1 2)")
               (assert (string= output-line "ok")))
              ((string= input-line "n")
               (assert (string= output-line "3")))
              ((string= input-line "(set! n (lambda (x) (+ x 1)))")
               (assert (string= output-line "ok")))
              ((string= input-line "(n 1)")
               (assert (string= output-line "2")))
              (t 'done)))))
  'done)

;; (execute-assignments-tests)

;; исполняет тесты функций
(defun execute-functions-tests()
  (load-functions-tests)
  (with-open-file (input_stream *input* :direction :input)
    (with-open-file (output_stream *output* :direction :output
                                   :if-exists :supersede)
      (sb-ext:run-program "./proga" (list '()) :wait t :input input_stream
                          :output output_stream)
      (force-output output_stream)))
  (with-open-file (output_stream *output* :direction :input)
    (with-open-file (input_stream *input* :direction :input)
      (do ((output-line (read-line output_stream nil 'eof)
                        (read-line output_stream nil 'eof))
           (input-line (read-line input_stream nil 'eof)
                       (read-line input_stream nil 'eof)))
          ((eql input-line 'eof))
        (cond ((string= input-line "(define (my-reverse lst)
                                       (define (my-reverse lst new-lst)
                                           (if (null? lst)
                                               new-lst
                                               (my-reverse (cdr lst) (cons (car lst) new-lst))))
                                     (my-reverse lst '()))")
               (assert (string= output-line "ok")))
              ((string= input-line "(my-reverse '(1 2 3 4))")
               (assert (string= output-line "(4 3 2 1)")))
              ((string= input-line "(my-reverse '(1 (2 3) 4))")
               (assert (string= output-line "(4 (2 3) 1)")))
              ((string= input-line "(my-reverse '(1 (2 3) 4) 'd)")
               (assert
                (string= output-line
                         "(COMPOUND PROCEDURE ERROR: too much args for procedure)")))
              ((string= input-line "(my-reverse \"d\")")
               (assert (string= output-line "(ERR CDR: arg isn't a pair)")))

              ((string= input-line "(define (factorial n)
                                       (if (= n 0)
                                           1
                                           (* n (factorial (- n 1)))))")
               (assert (string= output-line "ok")))
              ((string= input-line "(factorial 3)")
               (assert (string= output-line "6")))
              ((string= input-line "(factorial 6)")
               (assert (string= output-line "720")))

              ((string= input-line "(define (fib n)
                                         (cond ((= n 0) 0)
                                               ((= n 1) 1)
                                               (else (+ (fib (- n 1))
                                                        (fib (- n 2))))))")
               (assert (string= output-line "ok")))
              ((string= input-line "(fib 8)")
               (assert (string= output-line "21")))
              (t 'done)))))
  'done)

;; (execute-functions-tests)

;; исполняет все тесты
(defun execute-all-tests()
  (execute-arithmetic-tests)
  (execute-definitions-tests)
  (execute-assignments-tests)
  (execute-functions-tests))

;; (time
;;  (execute-all-tests)
;;  )

;; запуск всех тестов
(execute-all-tests)
