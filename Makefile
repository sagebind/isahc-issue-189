.PHONY: lldb-c
lldb-c: target/c/main
	lldb --batch --source lldb-run.batch --source-on-crash lldb-crash.batch $<

.PHONY: lldb-rust
lldb-rust: target/debug/isahc-issue-189
	lldb --batch --source lldb-run.batch --source-on-crash lldb-crash.batch $<

target/c/main: main.c
	mkdir -p $(@D)
	clang -g -O0 -o $@ -lcurl $<

target/debug/isahc-issue-189: cargo-build

.PHONY: cargo-build
cargo-build:
	cargo build
