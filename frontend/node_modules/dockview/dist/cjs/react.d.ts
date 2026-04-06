import React from 'react';
import { IFrameworkPart, DockviewIDisposable } from 'dockview-core';
export interface ReactPortalStore {
    addPortal: (portal: React.ReactPortal) => DockviewIDisposable;
}
export declare const ReactPartContext: React.Context<{}>;
export declare class ReactPart<P extends object, C extends object = {}> implements IFrameworkPart {
    private readonly parent;
    private readonly portalStore;
    private readonly component;
    private readonly parameters;
    private readonly context?;
    private _initialProps;
    private componentInstance?;
    private ref?;
    private disposed;
    constructor(parent: HTMLElement, portalStore: ReactPortalStore, component: React.FunctionComponent<P>, parameters: P, context?: C | undefined);
    update(props: {
        [index: string]: any;
    }): void;
    private createPortal;
    dispose(): void;
}
type PortalLifecycleHook = () => [
    React.ReactPortal[],
    (portal: React.ReactPortal) => DockviewIDisposable
];
/**
 * A React Hook that returns an array of portals to be rendered by the user of this hook
 * and a disposable function to add a portal. Calling dispose removes this portal from the
 * portal array
 */
export declare const usePortalsLifecycle: PortalLifecycleHook;
export declare function isReactComponent(component: any): boolean;
export {};
