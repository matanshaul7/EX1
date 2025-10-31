# URL Bloom Filter Project

## About The Project

This project is a simple URL filtering system using a **Bloom filter**, a fast and memory-efficient data structure. It helps check if a URL is blacklisted without saving all URLs individually.
The project was built using **Test Driven Development (TDD)** and a clean **SOLID-based C++ design**.

Our Jira project link: [https://ilaip100.atlassian.net/jira/software/projects/AE/summary](https://ilaip100.atlassian.net/jira/software/projects/AE/summary) 

## How It Works

The program reads an initial config line: `[bit array size] [list of hash functions]` 

Then:
* If the input line is of the form: `1 [URL]`, add the URL to the blacklist.
* If the input line is of the form: `2 [URL]`, check if the URL is blacklisted.

The Bloom filter is saved automatically to a file and reloaded on program restart.
It's important to note: **False positives can happen**, but **false negatives cannot**.

## How To Run

Make sure you have Docker and Docker Compose installed.

### Build the Image
To build the images, write this command line:
```bash
docker build -t bloom_filter_app .
```

### Run the App
To run the app, write this command line:
```bash
docker run --rm -it bloom_filter_app
```

### Run the Unit Tests
To run the unit tests, write this command line:
```bash
docker run bloom-filter ./runTests
```

## Example Usage

```
8 2
1 www.example.com0
2 www.example.com0
true true
2 www.example.com4
true false
```

## Screenshots

### Program Start and Config Input

```
PS C:\Users\Matan-Laptop\Bar Ilan\GitHub\Ex1\EX1> docker run -it bloom-filter
8 1 2
```

### Adding a URL

```
1 www.firstproject.com
1 otc
```

### Checking a URL

```
2 www.firstproject.com
true true
2 otc
false
```

*When checking if 'otc' is in the bloomfilter, it returns false because 'otc' is not in a valid URL format.* 

### Running tests on Docker

```
PS C:\Users\Matan-Laptop\Bar Ilan\GitHub\Ex1\EX1> docker run bloom-filter ./runTests
[ ] Running 43 tests from 3 test suites.
 Global test environment set-up.
 12 tests from BloomFilterTest
[ OK ] BloomFilterTest.AddAndContains ( ms)
[ OK ] BloomFilterTest.ContainsAbsolutely (0 ms)
[ OK ] BloomFilterTest.DoesNotContain (0 ms)
...
[--] 12 tests from BloomFilterTest (0 ms total)
[--] 16 tests from InputManagerTest
[ OK ] InputManagerTest.ConvertLine_AddToBlacklist_ValidURL (1 ms)
[ OK ] InputManagerTest.ConvertLine_AddToBlacklist_MissingURL (0 ms)
[ OK ] InputManagerTest.ConvertLine_CheckBlacklist_ValidURL (0 ms)
...
[--] 16 tests from InputManagerTest (7 ms total)
[--] 15 tests from MainTest
[ OK ] MainTest.ValidInput (8 ms)
[ OK ] MainTest.InvalidFirstLine (e ms)
[ OK ] MainTest.NoConversionForEmptyLine (0 ms)
...
[--] 15 tests from MainTest (0 ms total)
[--] Global test environment teardown.
[ ] 43 tests from 3 test suites ran. (8 ms total)
[PASSED] 43 tests.
PS C:\Users\Matan-Laptop\Bar Ilan\GitHub\Ex1\EX1>
```
### Stopping the App
To stop the app, simply press `Ctrl + D` in the terminal where the Docker container is running.
