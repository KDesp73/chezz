# chess

Chess Library and 2 player game written in C 

![image](https://github.com/user-attachments/assets/4e377992-daeb-4152-9592-5066781b246a)

## Installation

### Installation Directories

Libraries: `/usr/lib`

Headers: `/usr/local/include/chess`

```bash
# 1. Clone
git clone https://github.com/KDesp73/chezz
cd chezz

# 2. Verify that all the tests are passing (optional)
make test

# 3. Install locally
make install
```

## Examples

See [examples/](https://github.com/KDesp73/chezz/tree/main/examples)

## Specifics

There are two board and move representations:

- 2D character array as the board and coords based squares
- Board made of 64-bit integers (bitboards) and 0-63 integers as the squares

For differenciation purposes the first representation uses `snake_case` for type
and function naming, while the 2nd one uses `PascalCase`

## License

[MIT](./LICENSE)
