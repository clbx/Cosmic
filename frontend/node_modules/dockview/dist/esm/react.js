import React from 'react';
import ReactDOM from 'react-dom';
import { DockviewDisposable, } from 'dockview-core';
/**
 * This component is intended to interface between vanilla-js and React hence we need to be
 * creative in how we update props.
 * A ref of the component is exposed with an update method; which when called stores the props
 * as a ref within this component and forcefully triggers a re-render of the component using
 * the ref of props we just set on the renderered component as the props passed to the inner
 * component
 */
const ReactComponentBridge = (props, ref) => {
    const [_, triggerRender] = React.useState();
    const _props = React.useRef(props.componentProps);
    React.useImperativeHandle(ref, () => ({
        update: (componentProps) => {
            _props.current = Object.assign(Object.assign({}, _props.current), componentProps);
            /**
             * setting a arbitrary piece of state within this component will
             * trigger a re-render.
             * we use this rather than updating through a prop since we can
             * pass a ref into the vanilla-js world.
             */
            triggerRender(Date.now());
        },
    }), []);
    return React.createElement(props.component, _props.current);
};
ReactComponentBridge.displayName = 'DockviewReactJsBridge';
/**
 * Since we are storing the React.Portal references in a rendered array they
 * require a key property like any other React element rendered in an array
 * to prevent excessive re-rendering
 */
const uniquePortalKeyGenerator = (() => {
    let value = 1;
    return { next: () => `dockview_react_portal_key_${(value++).toString()}` };
})();
export const ReactPartContext = React.createContext({});
export class ReactPart {
    constructor(parent, portalStore, component, parameters, context) {
        this.parent = parent;
        this.portalStore = portalStore;
        this.component = component;
        this.parameters = parameters;
        this.context = context;
        this._initialProps = {};
        this.disposed = false;
        this.createPortal();
    }
    update(props) {
        if (this.disposed) {
            throw new Error('invalid operation: resource is already disposed');
        }
        if (!this.componentInstance) {
            // if the component is yet to be mounted store the props
            this._initialProps = Object.assign(Object.assign({}, this._initialProps), props);
        }
        else {
            this.componentInstance.update(props);
        }
    }
    createPortal() {
        if (this.disposed) {
            throw new Error('invalid operation: resource is already disposed');
        }
        if (!isReactComponent(this.component)) {
            /**
             * we know this isn't a React.FunctionComponent so throw an error here.
             * if we do not intercept then React library will throw a very obsure error
             * for the same reason... at least at this point we will emit a sensible stacktrace.
             */
            throw new Error('Dockview: Only React.memo(...), React.ForwardRef(...) and functional components are accepted as components');
        }
        const bridgeComponent = React.createElement(React.forwardRef(ReactComponentBridge), {
            component: this
                .component,
            componentProps: this.parameters,
            ref: (element) => {
                this.componentInstance = element;
                if (Object.keys(this._initialProps).length > 0) {
                    this.componentInstance.update(this._initialProps);
                    this._initialProps = {}; // don't keep a reference to the users object once no longer required
                }
            },
        });
        const node = this.context
            ? React.createElement(ReactPartContext.Provider, { value: this.context }, bridgeComponent)
            : bridgeComponent;
        const portal = ReactDOM.createPortal(node, this.parent, uniquePortalKeyGenerator.next());
        this.ref = {
            portal,
            disposable: this.portalStore.addPortal(portal),
        };
    }
    dispose() {
        var _a;
        (_a = this.ref) === null || _a === void 0 ? void 0 : _a.disposable.dispose();
        this.disposed = true;
    }
}
/**
 * A React Hook that returns an array of portals to be rendered by the user of this hook
 * and a disposable function to add a portal. Calling dispose removes this portal from the
 * portal array
 */
export const usePortalsLifecycle = () => {
    const [portals, setPortals] = React.useState([]);
    React.useDebugValue(`Portal count: ${portals.length}`);
    const addPortal = React.useCallback((portal) => {
        setPortals((existingPortals) => [...existingPortals, portal]);
        let disposed = false;
        return DockviewDisposable.from(() => {
            if (disposed) {
                throw new Error('invalid operation: resource already disposed');
            }
            disposed = true;
            setPortals((existingPortals) => existingPortals.filter((p) => p !== portal));
        });
    }, []);
    return [portals, addPortal];
};
export function isReactComponent(component) {
    /**
     * Yes, we could use "react-is" but that would introduce an unwanted peer dependency
     * so for now we will check in a rather crude fashion...
     */
    return (typeof component === 'function' /** Functional Componnts */ ||
        !!(component === null || component === void 0 ? void 0 : component.$$typeof) /** React.memo(...) Components */);
}
