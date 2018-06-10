@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = sext i8 %0 to i32
	ret i32 %2
}

define i1 @intToBool(i32) {
	%2 = icmp eq i32 %0, 0
	ret i1 %2
}

define i32 @boolToInt(i1) {
	%2 = sext i1 %0 to i32
	ret i32 %2
}

define float @intToFloat(i32) {
	%2 = sitofp i32 %0 to float
	ret float %2
}

define i32 @floatToInt(float) {
	%2 = fptosi float %0 to i32
	ret i32 %2
}

define i32 @printNum(i32) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)
	ret i32 0
}
define i32 @printFloat(float) {
	%2 = fpext float %0 to double
	%3 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printFloat.constStr, i32 0, i32 0), double %2)
	ret i32 0
}

define i32 @printStr(i8*) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}

define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

@str.array = common global [100 x i8] zeroinitializer
@str = internal global i8* getelementptr inbounds ([100 x i8], [100 x i8]* @str.array, i32 0, i32 0)
@length = common global i32 0
@position = common global i32 0

define i8 @current() {
entry:
  %result = alloca i8
  store i8 0, i8* %result
  %load.val = load i32, i32* @position
  %load.val1 = load i32, i32* @length
  %gt.res = icmp sgt i32 %load.val, %load.val1
  %not.res = icmp eq i1 %gt.res, false
  br i1 %not.res, label %if.take, label %if.fail

if.take:                                          ; preds = %entry
  %load = load i32, i32* @position
  %array.load = load i8*, i8** @str
  %visit = getelementptr inbounds i8, i8* %array.load, i32 %load
  %load.val2 = load i8, i8* %visit
  store i8 %load.val2, i8* %result
  %ret.load = load i8, i8* %result
  ret i8 %ret.load
  br label %if.merge

if.fail:                                          ; preds = %entry
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  store i8 0, i8* %result
  %ret.load3 = load i8, i8* %result
  ret i8 %ret.load3
}

define i32 @match() {
entry:
  %result = alloca i32
  store i32 0, i32* %result
  %load.val = load i32, i32* @position
  %add.res = add i32 %load.val, 1
  store i32 %add.res, i32* @position
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseNum() {
entry:
  %c = alloca i8
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i8 @current()
  store i8 %call.res, i8* %c
  store i32 0, i32* %result
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %load.val = load i8, i8* %c
  %lt.res = icmp slt i8 %load.val, 48
  %not.res = icmp eq i1 %lt.res, false
  %load.val1 = load i8, i8* %c
  %gt.res = icmp sgt i8 %load.val1, 57
  %not.res2 = icmp eq i1 %gt.res, false
  %and.res = and i1 %not.res, %not.res2
  br i1 %and.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val3 = load i32, i32* %result
  %mul.res = mul i32 %load.val3, 10
  %load.val4 = load i8, i8* %c
  %sext.temp = sext i8 %load.val4 to i32
  %add.res = add i32 %mul.res, %sext.temp
  %sub.res = sub i32 %add.res, 48
  store i32 %sub.res, i32* %result
  %call.res5 = call i32 @match()
  %call.res6 = call i8 @current()
  store i8 %call.res6, i8* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseFactor() {
entry:
  %temp = alloca i32
  %c = alloca i8
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i8 @current()
  store i8 %call.res, i8* %c
  %load.val = load i8, i8* %c
  %lt.res = icmp eq i8 %load.val, 40
  br i1 %lt.res, label %if.take, label %if.fail

if.take:                                          ; preds = %entry
  %call.res1 = call i32 @match()
  %call.res2 = call i32 @parseExpr()
  store i32 %call.res2, i32* %temp
  %call.res3 = call i32 @match()
  %load.val4 = load i32, i32* %temp
  store i32 %load.val4, i32* %result
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
  br label %if.merge

if.fail:                                          ; preds = %entry
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res5 = call i32 @parseNum()
  store i32 %call.res5, i32* %result
  %ret.load6 = load i32, i32* %result
  ret i32 %ret.load6
}

define i32 @parseTerm() {
entry:
  %c = alloca i8
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i32 @parseFactor()
  store i32 %call.res, i32* %result
  %call.res1 = call i8 @current()
  store i8 %call.res1, i8* %c
  br label %while.cond

while.cond:                                       ; preds = %if.merge, %entry
  %load.val = load i8, i8* %c
  %lt.res = icmp eq i8 %load.val, 42
  %load.val2 = load i8, i8* %c
  %lt.res3 = icmp eq i8 %load.val2, 47
  %or.res = or i1 %lt.res, %lt.res3
  br i1 %or.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val4 = load i8, i8* %c
  %lt.res5 = icmp eq i8 %load.val4, 42
  br i1 %lt.res5, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body
  %call.res6 = call i32 @match()
  %call.res7 = call i32 @parseFactor()
  %load.val8 = load i32, i32* %result
  %mul.res = mul i32 %load.val8, %call.res7
  store i32 %mul.res, i32* %result
  br label %if.merge

if.fail:                                          ; preds = %while.body
  %call.res9 = call i32 @match()
  %call.res10 = call i32 @parseFactor()
  %load.val11 = load i32, i32* %result
  %div.res = sdiv exact i32 %load.val11, %call.res10
  store i32 %div.res, i32* %result
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res12 = call i8 @current()
  store i8 %call.res12, i8* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseExpr() {
entry:
  %c = alloca i8
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i32 @parseTerm()
  store i32 %call.res, i32* %result
  %call.res1 = call i8 @current()
  store i8 %call.res1, i8* %c
  br label %while.cond

while.cond:                                       ; preds = %if.merge, %entry
  %load.val = load i8, i8* %c
  %lt.res = icmp eq i8 %load.val, 43
  %load.val2 = load i8, i8* %c
  %lt.res3 = icmp eq i8 %load.val2, 45
  %or.res = or i1 %lt.res, %lt.res3
  br i1 %or.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val4 = load i8, i8* %c
  %lt.res5 = icmp eq i8 %load.val4, 43
  br i1 %lt.res5, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body
  %call.res6 = call i32 @match()
  %call.res7 = call i32 @parseTerm()
  %load.val8 = load i32, i32* %result
  %add.res = add i32 %load.val8, %call.res7
  store i32 %add.res, i32* %result
  br label %if.merge

if.fail:                                          ; preds = %while.body
  %call.res9 = call i32 @match()
  %call.res10 = call i32 @parseTerm()
  %load.val11 = load i32, i32* %result
  %sub.res = sub i32 %load.val11, %call.res10
  store i32 %sub.res, i32* %result
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res12 = call i8 @current()
  store i8 %call.res12, i8* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @main() {
entry:
  %res = alloca i32
  %c = alloca i8
  br label %while.cond

while.cond:                                       ; preds = %while.merge, %entry
  br i1 true, label %while.body, label %while.merge15

while.body:                                       ; preds = %while.cond
  store i32 0, i32* @length
  store i32 0, i32* @position
  %call.res = call i32 @getchar()
  %trunc.temp = trunc i32 %call.res to i8
  store i8 %trunc.temp, i8* %c
  br label %while.cond1

while.cond1:                                      ; preds = %if.merge, %if.take, %while.body
  %load.val = load i8, i8* %c
  %sext.temp = sext i8 %load.val to i32
  %lt.res = icmp eq i32 %sext.temp, 10
  %not.res = icmp eq i1 %lt.res, false
  br i1 %not.res, label %while.body2, label %while.merge

while.body2:                                      ; preds = %while.cond1
  %load.val3 = load i8, i8* %c
  %lt.res4 = icmp eq i8 %load.val3, 32
  br i1 %lt.res4, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body2
  %call.res5 = call i32 @getchar()
  %trunc.temp6 = trunc i32 %call.res5 to i8
  store i8 %trunc.temp6, i8* %c
  br label %while.cond1
  br label %if.merge

if.fail:                                          ; preds = %while.body2
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %load = load i32, i32* @length
  %array.load = load i8*, i8** @str
  %visit = getelementptr inbounds i8, i8* %array.load, i32 %load
  %load.val7 = load i8, i8* %c
  store i8 %load.val7, i8* %visit
  %load.val8 = load i32, i32* @length
  %add.res = add i32 %load.val8, 1
  store i32 %add.res, i32* @length
  %call.res9 = call i32 @getchar()
  %trunc.temp10 = trunc i32 %call.res9 to i8
  store i8 %trunc.temp10, i8* %c
  br label %while.cond1

while.merge:                                      ; preds = %while.cond1
  %call.res11 = call i32 @parseExpr()
  store i32 %call.res11, i32* %res
  %load12 = load i32, i32* %res
  %call.res13 = call i32 @printNum(i32 %load12)
  %call.res14 = call i32 @newline()
  br label %while.cond

while.merge15:                                    ; preds = %while.cond
  ret i32 0
}
