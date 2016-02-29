
import unittest
import numpy as np

from binsearch import binary_search

class MyTest(unittest.TestCase):
    
    def test_mymath(self):
        # Test empty array
        self.assertEqual(binary_search([], 2),  -1)
        # Test array with nans
        self.assertEqual(binary_search(['nan', 1, 2], 2), 2)
        # Test array of length 1
        self.assertEqual(binary_search([1], 1), 0)
        # Test array of length 2
        self.assertEqual(binary_search([0,10], 10), 1)
        # Test needle greater than the range of the array
        self.assertEqual(binary_search(range(10), 11), -1)
        # Test needle at extreme of array
        self.assertEqual(binary_search(range(5), 4), 4)

if __name__ == '__main__':
    unittest.main()