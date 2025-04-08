import os
import sys
import unittest

import jax.numpy as jnp
import numpy as np
from jax import random
from sklearn.decomposition import TruncatedSVD as SklearnSVD

import spu.libspu as libspu
import spu.utils.simulation as spsim

# Add the sml directory to the path
sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))

from sml.decomposition.jacobi_svd import jacobi_svd

np.random.seed(0)


class UnitTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        print(" ========= start test of jacobi_svd package ========= \n")

        # 1. init sim
        cls.sim64 = spsim.Simulator.simple(
            3, libspu.ProtocolKind.ABY3, libspu.FieldType.FM64
        )
        config128 = libspu.RuntimeConfig(
            protocol=libspu.ProtocolKind.ABY3,
            field=libspu.FieldType.FM128,
            fxp_fraction_bits=30,
        )
        cls.sim128 = spsim.Simulator(3, config128)

    def test_jacobi_svd(self):
        print("start test jacobi svd.")

        # Test fit_transform
        def proc_transform(A):
            U, singular_values, V_T = jacobi_svd(A)
            return U, singular_values, V_T

        # Create a random dataset
        A = random.normal(random.PRNGKey(0), (10, 10))
        A = (A + A.T) / 2

        # Run the simulation
        results = spsim.sim_jax(self.sim128, proc_transform)(A)

        A_np = np.array(A)

        # Run fit_transform using sklearn
        sklearn_svd = SklearnSVD(n_components=min(A_np.shape))
        sklearn_svd.fit(A_np)
        singular_values_sklearn = sklearn_svd.singular_values_
        singular_matrix_sklearn = sklearn_svd.components_
        
        # Compare the results
        np.testing.assert_allclose(singular_values_sklearn, results[1], rtol=0.01, atol=0.01)
        np.testing.assert_allclose(np.abs(singular_matrix_sklearn), np.abs(results[2]), rtol=0.01, atol=0.01)

if __name__ == "__main__":
    unittest.main()
