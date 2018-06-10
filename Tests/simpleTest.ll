
@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = alloca i8, align 1
	store i8 %0, i8* %2, align 1
	%3 = load i8, i8* %2, align 1
	%4 = sext i8 %3 to i32
	ret i32 %4
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

define i32 @readNum() {
	%1 = alloca i32, align 4
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)
	%3 = load i32, i32* %1, align 4
	ret i32 %3
}
define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}

define i32 @readStr(i8*)  {
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

define i32 @main(i32) {
entry:
  %c = alloca i1
  %b = alloca i32
  %a = alloca i32
  store i32 %0, i32* %a
  store i32 1, i32* %b
  store i1 false, i1* %c
  %load.val = load i32, i32* %a
  %load.val1 = load i32, i32* %b
  %add.res = add i32 %load.val, %load.val1
  store i32 %add.res, i32* %b
  %load.val2 = load i32, i32* %b
  %load.val3 = load i32, i32* %a
  %gt.res = icmp sgt i32 %load.val2, %load.val3
  store i1 %gt.res, i1* %c
  %if.cond.load = load i1, i1* %c
  br i1 %if.cond.load, label %if.take, label %if.fail

if.take:                                          ; preds = %entry
  store i32 5, i32* %b
  br label %if.merge

if.fail:                                          ; preds = %entry
  store i32 3, i32* %b
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %load = load i32, i32* %b
  %call.res = call i32 @printNum(i32 %load)
  %ret.load = load i32, i32* %b
  ret i32 %ret.load
}
