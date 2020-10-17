(defparameter *input* "input.txt")
(defparameter *output* "output.txt")

;; принимает строку выражения и строку предполагаемого результата
;; вычисляет выражение и сравнивает полученный результат с предполагаемым
(defmacro exp-test(exp-string expected-result-string)
  `(let ((proc (sb-ext:run-program "./proga" (list '()) :wait nil :input :stream
                                   :output :stream)))
     (with-open-stream (input (sb-ext:process-input proc))
       (with-open-stream (output (sb-ext:process-output proc))
         (format input "~A ~%" ,exp-string)
         (format input "(quite)~%")
         (force-output input)
         (force-output output)
         (let ((output-line (read-line output nil 'eof)))
           (assert (string= output-line ,expected-result-string)))))))

;; (exp-test "3" "(+ 1 2)")

;; принимает список выражений и список предполагаемых рехультатов
;; исполняет выражение одно за другим, сравнивая полученный результат с предполагаемым
(defun multiple-exp-test-rec (input output expected-result-strings exp-strings)
  (if (null exp-strings)
      (progn
        (format input "(quite)~%")
        (force-output input))
      (let ((exp (car exp-strings))
            (expected-result (car expected-result-strings)))
        (format input "~A ~%" exp)
        (force-output input)
        (force-output output)
        (let ((result (read-line output nil 'eof)))
          ;; (format t "result ~A ~%" result)
          (assert (string= result expected-result))
          (multiple-exp-test-rec input output (cdr expected-result-strings)
                                 (cdr exp-strings))))))


(defun multiple-exp-test(exp-strings expected-result-strings)
  (let ((proc (sb-ext:run-program "./proga" (list '()) :wait nil :input :stream
                                  :output :stream)))
    (with-open-stream (input (sb-ext:process-input proc))
      (with-open-stream (output (sb-ext:process-output proc))
        (multiple-exp-test-rec input output expected-result-strings exp-strings)))))


;; (multiple-exp-test '("(+ 1 2 3)" "(+ 3 4)" "(+ 7 8)") '("6" "7" "15"))

(defun add_test()
  (exp-test "(+ 1 2)" "3"))

(add_test)
(defun add_test_without_args()
  (exp-test  "(+)" "0"))

(defun add_test_with_one_arg()
  (exp-test "(+ 10)" "10"))

(defun multiple_add_test()
  (exp-test "(+ 1 2 3 4 5 6 7)" "28"))

(defun multiple_add_test_with_quoted_exp()
  (exp-test "(+ 1 2 3 4 5 6 's 7)" "(ADD ERROR: add gets only numbers)"))

(defun multiple_add_test_with_unusigned_var()
  (exp-test  "(+ 1 2 3 4 5 6 s 7)"
             "(ERR LOOKUP_VARIABLE: unussigned variable)"))

(defun sub_test()
  (exp-test "(- 1 2)" "-1"))

(defun multiple_sub_test()
  (exp-test "(- 10 6 2)" "2"))

(defun sub_test_without_args()
  (exp-test "(-)" "(SUB ERROR: sub needs at least 1 arg)"))

(defun sub_test_with_one_args()
  (exp-test "(- 3)" "-3"))

(defun mul_test()
  (exp-test "(* 3 2)" "6"))

(defun multiple_mul_test()
  (exp-test "(* 3 5 2)" "30"))

(defun multiple_mul_test_with_quoted_exp()
  (exp-test "(* 3 5 's 2)" "(MUL ERROR: mul gets only numbers)"))

(defun multiple_mul_test_with_list()
  (exp-test "(* 3 5 '(1 2 3) 2)" "(MUL ERROR: mul gets only numbers)"))

(defun mul_test_without_args()
  (exp-test "(*)" "1"))

(defun mul_test_with_one_arg()
  (exp-test "(* 8)" "8"))

(defun division_test()
  (exp-test "(/ 3 2)" "1"))

(defun multiple_division_test()
  (exp-test "(/ 8 2 2 1)" "2"))

(defun division_test_by_zero()
  (exp-test "(/ 8 0)" "(DIVISION ERROR: division by zero)"))

(defun division_test_without_args()
  (exp-test "(/)" "(DIVISION ERROR: division needs at least 2 args)"))

(defun division_test_with_one_arg()
  (exp-test "(/ 1)" "(DIVISION ERROR: division needs at least 2 args)"))

(defun multiple_operations_arithmetic_test()
  (exp-test "(+ (* 1 2 3) (/ 9 (- 12 9)))" "9"))

(defun run-arithmetic-tests()
  (add_test)
  (add_test_without_args)
  (add_test_with_one_arg)
  (multiple_add_test)
  (multiple_add_test_with_quoted_exp)
  (multiple_add_test_with_unusigned_var)
  (sub_test)
  (multiple_sub_test)
  (sub_test_without_args)
  (sub_test_with_one_args)
  (mul_test)
  (multiple_mul_test)
  (multiple_mul_test_with_quoted_exp)
  (multiple_mul_test_with_list)
  (mul_test_without_args)
  (mul_test_with_one_arg)
  (division_test)
  (multiple_division_test)
  (division_test_by_zero)
  (division_test_without_args)
  (division_test_with_one_arg))

(run-arithmetic-tests)

(defun simple_definition_test()
  (multiple-exp-test'("(define n 2)" "n") '("ok" "2")))

(defun definition_test_with_evaluating_value()
  (multiple-exp-test '("(define n (+ 1 2 3))" "n") '("ok" "6")))

(defun definition_test_with_lambda()
  (multiple-exp-test '("(define a (lambda (x) (+ x 1)))" "(a 3)")
                     '("ok" "4")))

(defun run-definition-tests()
  (simple_definition_test)
  (definition_test_with_evaluating_value)
  (definition_test_with_lambda))

(run-definition-tests)


(defun simple_assignment_test()
  (multiple-exp-test'("(define n 2)"
                      "n"
                      "(set! n (+ 1 2))"
                      "n"
                      "(set! n (lambda (x) (+ x 1)))"
                      "(n 4)")
                    '("ok"
                      "2"
                      "ok"
                      "3"
                      "ok"
                      "5")))

(defun run-assignment_test()
  (simple_assignment_test))
(run-assignment_test)

(defun fn-my-reverse-test()
  (multiple-exp-test '("(define (my-reverse lst)
                             (define (my-reverse lst new-lst)
                                 (if (null? lst)
                                     new-lst
                                     (my-reverse (cdr lst) (cons (car lst) new-lst))))
                           (my-reverse lst '()))"
                       "(my-reverse '(1 2 3 4))"
                       "(my-reverse '(1 (2 3) 4))"
                       "(my-reverse '(1 (2 3) 4) 'd)"
                       "(my-reverse \"d\")")
                     '("ok"
                       "(4 3 2 1)"
                       "(4 (2 3) 1)"
                       "(COMPOUND PROCEDURE ERROR: too much args for procedure)"
                       "(ERR CDR: arg isn't a pair)")))

(defun fn-factorial-test()
  (multiple-exp-test '("(define (factorial n)
                             (if (= n 0)
                                 1
                                 (* n (factorial (- n 1)))))"
                       "(factorial 3)"
                       "(factorial 6)")
                     '("ok"
                       "6"
                       "720")))

(defun fn-fib-test()
  (multiple-exp-test '("(define (fib n)
                             (cond ((= n 0) 0)
                                   ((= n 1) 1)
                                   (else (+ (fib (- n 1))
                                            (fib (- n 2))))))"
                       "(fib 8)"
                       "(fib 0)")
                     '("ok"
                       "21"
                       "0")))

(defun run-fn-tests()
  (fn-fib-test)
  (fn-factorial-test)
  (fn-my-reverse-test))

;; (run-fn-tests)

(defun run-all-tests()
  (run-arithmetic-tests)
  (run-assignment_test)
  (run-definition-tests)
  (run-fn-tests))

(run-all-tests)
