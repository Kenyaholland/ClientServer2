package main

import (
	"fmt"
	"log"
	"math/rand"
	"net/http"
	"os"
	"sort"
	"strconv"
	"time"
)

var letters = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")

var hrefPage = `<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8"/>
		<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
		<title>Highly sensitive data</title>
	</head>
	<body>
		<a href="/%s">%v</a>
	</body>
</html>`

var pPage = `<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8"/>
		<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
		<title>Highly sensitive data</title>
	</head>
	<body>
		<p>%s</p>
	</body>
</html>`

func randomSeq(numElements int, elementLen int) []string {
	elements := make([]string, numElements)
	for i := range elements {
		seq := make([]rune, elementLen)
		for j := range seq {
			seq[j] = letters[rand.Intn(len(letters))]
		}
		elements[i] = string(seq)
	}
	return elements
}

// Returns an array which contains the indices sorted according to pages.
func constructIndex(pages []string) []int {
	indices := make([]int, len(pages))
	for i := range indices {
		indices[i] = i
	}
	sort.Slice(indices, func(i, j int) bool {
		return pages[indices[i]] < pages[indices[j]]
	})
	return indices
}

func binarySearch(index []int, pages []string, pageID string) int {
	lower := 0
	upper := len(index) - 1
	for upper-lower > 0 {
		middle := (upper + lower) / 2
		if pages[index[middle]] == pageID {
			return index[middle]
		}
		if pages[index[middle]] > pageID {
			upper = middle
		} else {
			lower = middle + 1
		}
	}
	if pages[index[upper]] == pageID {
		return index[upper]
	}
	return -1
}

// call like testServer.go <port> <numElements> <message>
func main() {
	rand.Seed(time.Now().UTC().UnixNano())
	port, _ := strconv.Atoi(os.Args[1])
	numElements, _ := strconv.Atoi(os.Args[2])
	pages := randomSeq(numElements, 100)
	index := constructIndex(pages)
	secretMessage := os.Args[3]

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		path := r.URL.Path[1:]
		pageIndex := binarySearch(index, pages, path)
		if pageIndex == len(pages)-1 {
			fmt.Fprintf(w, pPage, secretMessage)
		} else if pageIndex != -1 {
			fmt.Fprintf(w, hrefPage, pages[pageIndex+1], pageIndex+1)
		} else {
			fmt.Fprintf(w, hrefPage, pages[0], 0)
		}

	})

	fmt.Printf("Starting server at port %v\n", port)
	if err := http.ListenAndServe(":"+strconv.Itoa(port), nil); err != nil {
		log.Fatal(err)
	}
}
