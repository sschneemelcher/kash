# kash 🐚

### C posix shell

✨ A Fast and Fun Shell Implementation! ✨

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/sschneemelcher/kash/blob/main/LICENSE)

## Design Philosophy 🚀

1. ⚡️ `fast is better than slow` ⚡️

## Features 🌟

+ ✍️ Line editing - Enjoy easy command editing and modification.
+ 🚀 Command execution - Execute commands and experience speedy results.
+ 📂 Changing directory - Navigate seamlessly through directories.
+ 🔗 Command chaining with `;` - Combine multiple commands effortlessly.
+ 📥 Reading from file/stdin - Read input from files or standard input.
+ ⏳ Limited non-persistent history - Access previously executed commands.
+ 📜 `.kashrc` file - Customize your shell environment to suit your needs.
+ 🔄 Alias builtin for single commands - Create shortcuts for frequently used commands.
+ ↔️ Singular pipes - Connect commands using pipes for data flow.
+ 🎯 Completion for items in the current directory - Auto-complete file and directory names.

The goal of kash is to implement most of the POSIX standard and become the fastest shell to do so. This project is created for fun and to practice C programming, therefore, no guarantees on the timeline for completion! 😉

## Next TODOs 📝

+ 🚰 Longer piping and output redirection - Expand the functionality of pipes and redirections.
+ 🌐 Conditional chaining - Implement conditional logic for command execution.
+ 💼 Export builtin - Develop the `export` builtin command for manipulating environment variables.
+ 📚 Improve history - Enhance the history feature for better command recall.
+ 🔁 Loops and boolean expressions - Add support for loops and boolean expressions in scripts.

## Getting Started 🚀

To get started with kash, follow these steps:

1. Clone the repository:

   ```shell
   $ git clone https://github.com/sschneemelcher/kash.git
   $ cd kash
   ```

2. Build kash using the provided Makefile:

   ```shell
   $ make
   ```
3. Run kash:

   ```shell
   $ ./kash
   ```
4. Enjoy exploring and using kash! 🎉

## Contributing 🤝

Contributions are welcome! If you'd like to contribute to kash, please follow these guidelines:

1. Fork the repository.
1. Create a new branch.
1. Make your changes and commit them.
1. Push your changes to your forked repository.
1. Submit a pull request.

## License 📝

This project is licensed under the [MIT License](LICENSE).

If you find this project interesting or have any suggestions, feel free to reach out! 💌

Let's make kash the most exciting and powerful shell! 🚀💪
